#pragma once
/* RAFAELIA PIXEL_DESC_V1 — freestanding scalar C raster/geometry kernel.
 * No syscall, libc, malloc, dynamic allocation, float, heap or I/O.
 * RGB means unpremultiplied 8-bit channel samples (not ICC-managed print).
 * CMYK here is an explicitly idealized reversible numerical projection,
 * not a calibrated ink profile. PB is 1 bit represented by 0/1 bytes.
 * 16x16 descriptor semantics pinned to RAFAELIA Pixel Manifold V1:
 * 2026-09-26 (existing author's specification, not a new claim).
 */
#include "br_types.h"

typedef struct {u8 r,g,b;} BRPX_RGB;
typedef struct {u8 c,m,y,k;} BRPX_CMYK;
typedef struct {
    u8 mean_rgb[3],mean_y;
    u32 var_y;
    u16 grad_l1;
    u8 key[5];
    u16 samples;
} BRPX_Tile;
typedef struct {
    u32 count;
    u16 xmin,ymin,xmax,ymax;
    u32 cx_q8,cy_q8;
} BRPX_Geo;

/* Exact fixed-point luma formula from PIXEL_DESC_V1; no gamma correction.
 * It describes code-value luma Y', not physical light or perceived lightness. */
static u8 BRPX_LUMA(BRPX_RGB a){
    return (u8)(((u32)54u*a.r+(u32)183u*a.g+(u32)19u*a.b)>>8);
}
static u8 BRPX_PB(BRPX_RGB a,u8 threshold){
    return (u8)(BRPX_LUMA(a)>=threshold);
}
/* Algebraic ideal CMYK: K=255-maxRGB, C=(maxRGB-R)/maxRGB.
 * Round-to-nearest enables exact 8-bit RGB reconstruction in this model;
 * profiles, GCR/UCR, paper, inks and overprint are NOT represented. */
static BRPX_CMYK BRPX_TO_CMYK(BRPX_RGB a){
    u32 high=a.r;
    if((u32)a.g>high)high=a.g;
    if((u32)a.b>high)high=a.b;
    BRPX_CMYK b={0u,0u,0u,(u8)(255u-high)};
    if(high){
        b.c=(u8)((255u*(high-a.r)+high/2u)/high);
        b.m=(u8)((255u*(high-a.g)+high/2u)/high);
        b.y=(u8)((255u*(high-a.b)+high/2u)/high);
    }
    return b;
}
static BRPX_RGB BRPX_FROM_CMYK(BRPX_CMYK c){
    u32 high=255u-(u32)c.k;
    BRPX_RGB b={
        (u8)(((255u-c.c)*high+127u)/255u),
        (u8)(((255u-c.m)*high+127u)/255u),
        (u8)(((255u-c.y)*high+127u)/255u)
    };
    return b;
}
static u32 BRPX_ISQRT(u32 n){
    u32 rem=n,root=0u,bit=1u<<30;
    while(bit>rem)bit>>=2u;
    while(bit){
        if(rem>=root+bit){rem-=root+bit;root=(root>>1u)+bit;}
        else root>>=1u;
        bit>>=2u;
    }
    return root;
}
/* Input packed RGB/RGBA, stride=w*channels, no external references.
 * Reversible raster transforms 0/90/180/270 clockwise. Geometry changes
 * coordinate map but does not resample, re-color or premultiply alpha. */
static s32 BRPX_ROTATE(const u8*src,u32 w,u32 h,u32 channels,u32 turns,
                      u8*dst,u32 capacity){
    if(!src||!dst||src==dst||!w||!h||w>256u||h>256u||
       (channels!=3u&&channels!=4u)||turns>3u)return-1;
    u32 pixels=w*h,bytes=pixels*channels;
    if(pixels>65536u||capacity<bytes)return-1;
    u32 dw=(turns&1u)?h:w;
    for(u32 y=0u;y<h;y++)for(u32 x=0u;x<w;x++){
        u32 ox=x,oy=y;
        if(turns==1u){ox=h-1u-y;oy=x;}
        else if(turns==2u){ox=w-1u-x;oy=h-1u-y;}
        else if(turns==3u){ox=y;oy=w-1u-x;}
        u32 from=(y*w+x)*channels,to=(oy*dw+ox)*channels;
        for(u32 c=0u;c<channels;c++)dst[to+c]=src[from+c];
    }
    return (s32)bytes;
}
/* 1..16x1..16 packed RGB, explicit capacity in bytes. Integer variance
 * uses floor E[Y²] - (floor E[Y])², saturates grad to u16 contract.
 * key=[meanR>>5,meanG>>5,meanB>>5,min(7,isqrt(var)>>4),
 *      min(7,grad_l1>>4)]. Identical tiles yield identical keys,
 * NOT proof of common symbol, culture or source. */
static s32 BRPX_TILE16(const u8*src,u32 w,u32 h,u32 capacity,BRPX_Tile*out){
    if(!src||!out||!w||!h||w>16u||h>16u||capacity<w*h*3u)return-1;
    u32 count=w*h,sr=0u,sg=0u,sb=0u,sy=0u,sy2=0u,gradient=0u;
    for(u32 y=0u;y<h;y++)for(u32 x=0u;x<w;x++){
        u32 p=(y*w+x)*3u;
        BRPX_RGB c={src[p],src[p+1u],src[p+2u]};
        u32 yy=BRPX_LUMA(c);
        sr+=c.r;sg+=c.g;sb+=c.b;sy+=yy;sy2+=yy*yy;
        if(x){
            u32 q=(y*w+x-1u)*3u;
            BRPX_RGB left={src[q],src[q+1u],src[q+2u]};
            u32 other=BRPX_LUMA(left);
            gradient+=yy>=other?yy-other:other-yy;
        }
        if(y){
            u32 q=((y-1u)*w+x)*3u;
            BRPX_RGB above={src[q],src[q+1u],src[q+2u]};
            u32 other=BRPX_LUMA(above);
            gradient+=yy>=other?yy-other:other-yy;
        }
    }
    out->mean_rgb[0]=(u8)(sr/count);
    out->mean_rgb[1]=(u8)(sg/count);
    out->mean_rgb[2]=(u8)(sb/count);
    out->mean_y=(u8)(sy/count);
    u32 ysquare=(u32)out->mean_y*(u32)out->mean_y;
    u32 e2=sy2/count;
    out->var_y=e2>=ysquare?e2-ysquare:0u;
    out->grad_l1=(u16)(gradient>65535u?65535u:gradient);
    out->samples=(u16)count;
    out->key[0]=(u8)(out->mean_rgb[0]>>5u);
    out->key[1]=(u8)(out->mean_rgb[1]>>5u);
    out->key[2]=(u8)(out->mean_rgb[2]>>5u);
    u32 bucket=BRPX_ISQRT(out->var_y)>>4u;
    out->key[3]=(u8)(bucket>7u?7u:bucket);
    bucket=(u32)out->grad_l1>>4u;
    out->key[4]=(u8)(bucket>7u?7u:bucket);
    return 0;
}
/* PB geometry: byte mask 0 or 1, not color data. 0 foreground is valid,
 * empty selection returns 1 with count=0; malformed returns -1. All
 * centroids are arithmetic pixel-index means in Q8, not image moments. */
static s32 BRPX_GEOMETRY(const u8*mask,u32 w,u32 h,u32 capacity,BRPX_Geo*out){
    if(!mask||!out||!w||!h||w>256u||h>256u||capacity<w*h)return-1;
    u32 n=0u,sx=0u,sy=0u;
    u32 xmin=w,ymin=h,xmax=0u,ymax=0u;
    for(u32 y=0u;y<h;y++)for(u32 x=0u;x<w;x++){
        u8 v=mask[y*w+x];
        if(v>1u)return-1;
        if(!v)continue;
        n++;sx+=x;sy+=y;
        if(x<xmin)xmin=x;if(x>xmax)xmax=x;
        if(y<ymin)ymin=y;if(y>ymax)ymax=y;
    }
    out->count=n;
    if(!n){
        out->xmin=out->ymin=out->xmax=out->ymax=0u;
        out->cx_q8=out->cy_q8=0u;
        return 1; /* valid measured empty field; not missing evidence */
    }
    out->xmin=(u16)xmin;out->ymin=(u16)ymin;
    out->xmax=(u16)xmax;out->ymax=(u16)ymax;
    out->cx_q8=(sx<<8u)/n;out->cy_q8=(sy<<8u)/n;
    return 0;
}
