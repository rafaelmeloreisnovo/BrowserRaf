/* Host-only adversarial fixtures. The shipped freestanding binary still uses no libc.
 * No DNS, TCP, TLS or arbitrary URL requests are made by this test. */
#include <stdio.h>
#include <string.h>
#include "../internal/br_dns.h"
#include "../internal/br_html.h"

static unsigned checks=0u, failures=0u;
#define CHECK(x) do { checks++; if(!(x)){ failures++; \
    fprintf(stderr,"FAIL fixture line %d: %s\n",__LINE__,#x); }} while(0)

static void urls(void) {
    BCtx c;
    CHECK(URL_PARSE("http://example.com/a",&c)==0);
    CHECK(strcmp(c.host,"example.com")==0);
    CHECK(strcmp(c.path,"/a")==0 && c.port==80u && c.use_tls==0u);
    CHECK(URL_PARSE("https://example.com/",&c)==0 && c.use_tls==1u && c.port==443u);
    CHECK(URL_PARSE("http://127.0.0.1:65535/test",&c)==0 && c.port==65535u);
    CHECK(URL_PARSE("http://x:65536/",&c)!=0);
    CHECK(URL_PARSE("http://x:0/",&c)!=0);
    CHECK(URL_PARSE("http://x:abc/",&c)!=0);
    CHECK(URL_PARSE("http://u@x/",&c)!=0);
    CHECK(URL_PARSE("http://x/\r\nX-Evil: yes",&c)!=0);
    char long_host[350],long_path[620];
    memcpy(long_host,"http://",7);
    memset(long_host+7,'x',300);
    memcpy(long_host+307,"/\0",2);
    CHECK(URL_PARSE(long_host,&c)!=0);
    memcpy(long_path,"http://a/",9);
    memset(long_path+9,'z',580);long_path[589]=0;
    CHECK(URL_PARSE(long_path,&c)!=0);
    CHECK(URL_PARSE("",&c)!=0);
    CHECK(URL_PARSE((const char*)0,&c)!=0);
}

static void http(void) {
    u8 req[1024];
    u32 n=HTTP_BUILD_REQ("example.com","/a",req,sizeof(req));
    CHECK(n>0u && n<sizeof(req));
    CHECK(n>=15u && memcmp(req,"GET /a HTTP/1.1\r\n",17u)==0);
    CHECK(HTTP_BUILD_REQ("bad\r\nInjected:1","/",req,sizeof(req))==0u);
    CHECK(HTTP_BUILD_REQ("example.com","/\r\nX:y",req,sizeof(req))==0u);
    CHECK(HTTP_BUILD_REQ("example.com","/",req,5u)==0u);

    const char ok[]="HTTP/1.1 200 OK\r\nContent-Length: 4\r\n\r\nBODY";
    const char malformed[]="HTTP/1.1 200 OK\r\nContent-Length: 4x\r\n\r\nBODY";
    const char over[]="HTTP/1.1 200 OK\r\nContent-Length: 4294967296\r\n\r\n";
    const char duplicate[]="HTTP/1.1 200 OK\r\nContent-Length: 4\r\ncontent-length: 99\r\n\r\nBODY";
    const char incomplete[]="HTTP/1.1 200 OK\r\nContent-Length: 4\r\n";
    u32 len=0u;
    CHECK(HTTP_PARSE_STATUS((const u8*)ok,sizeof(ok)-1u)==200u);
    CHECK(HTTP_PARSE_STATUS((const u8*)"garbage 200 OK\r\n",16u)==0u);
    CHECK(HTTP_PARSE_STATUS((const u8*)"HTTP/9.9 200 OK\r\n",17u)==0u);
    CHECK(HTTP_CONTENT_LENGTH((const u8*)ok,sizeof(ok)-1u,&len)==1 && len==4u);
    CHECK(HTTP_CONTENT_LENGTH((const u8*)malformed,sizeof(malformed)-1u,&len)<0);
    CHECK(HTTP_CONTENT_LENGTH((const u8*)over,sizeof(over)-1u,&len)<0);
    CHECK(HTTP_CONTENT_LENGTH((const u8*)duplicate,sizeof(duplicate)-1u,&len)<0);
    CHECK(HTTP_CONTENT_LENGTH((const u8*)incomplete,sizeof(incomplete)-1u,&len)<0);
    CHECK(HTTP_HEADERS_END((const u8*)incomplete,sizeof(incomplete)-1u)==0u);
    u8 chunks[]="4\r\nWiki\r\n5\r\npedia\r\n0\r\n\r\n";
    u32 d=HTTP_DECHUNK(chunks,sizeof(chunks)-1u);
    CHECK(d==9u && memcmp(chunks,"Wikipedia",9u)==0);
    u8 bad[]="4\r\nWiki\r\n0\r\n";
    CHECK(HTTP_DECHUNK(bad,sizeof(bad)-1u)==0xFFFFFFFFu);
}

static void html(void) {
    u8 out[256];
    u8 sentinel=0xA5u;
    const u8 esc[]={'a',0x1B,'[','3','1','m','b',0x07,0x7F};
    CHECK(HTML_RENDER(esc,sizeof(esc),out,sizeof(out))>0u);
    CHECK(strchr((const char*)out,'a')!=0 && strchr((const char*)out,'b')!=0);
    /* Non-trusted raw ESC/BEL/DEL never pass to stdout. Renderer may emit its
     * own fixed reset ANSI at the end; avoid falsely banning that sequence. */
    CHECK(strstr((const char*)out,"a\033[31m")==0);
    CHECK(strchr((const char*)out,0x07)==0);
    CHECK(HTML_RENDER((const u8*)"<script>EVIL</script>HELLO",26u,out,sizeof(out))>0u);
    CHECK(strstr((const char*)out,"EVIL")==0);
    CHECK(strstr((const char*)out,"HELLO")!=0);
    CHECK(HTML_RENDER((const u8*)"x",1u,&sentinel,0u)==0u && sentinel==0xA5u);
    CHECK(HTML_RENDER((const u8*)"x",1u,out,1u)==0u && out[0]==0u);
    CHECK(HTML_RENDER((const u8*)0,3u,out,sizeof(out))==0u && out[0]==0u);
}

int main(void) {
    urls();http();html();
    fprintf(stderr,"BrowserRaf negative fixtures: %u checks, %u failed\n",checks,failures);
    return failures?1:0;
}
