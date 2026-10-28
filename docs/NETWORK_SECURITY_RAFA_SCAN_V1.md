# BrowserRaf RAFA-SCAN V1 — rota, DNS e TLS (candidate)

**STATUS: PROTOTYPE / PROMOTION=HOLD / ANDROID_PHYSICAL=TOKEN_VAZIO_NOT_RUN.**
Um diagnostico nao equivale a antivirus Android, auditoria de roteamento ponta-a-ponta ou HTTPS completo no BrowserRaf.

## Modos e privacidade

- `sh tools/rafascan_network.sh --offline` (padrao): le apenas `settings`, `getprop` e `ip` quando acessiveis. Sem DNS externo, ping, HTTP, root, logs persistentes ou alteracao de configuracoes.
- `sh tools/rafascan_network.sh --active` (opt-in): ate tres probes HTTPS, e ping opcional de tres amostras. Sistema: `https://example.com/`; DoH: mesmo dominio resolvido via `https://dns.google/dns-query`; IP direto: `--resolve one.one.one.one:443:1.1.1.1 https://one.one.one.one/`, com SNI/hostname e cadeia TLS verificados pelo curl. Sem redirecionamento, sem corpos baixados, sem proxy explicito, sem `--insecure`, tempos limitados.
- O modo ativo divulga IP de origem aos destinos e ao resolvedor escolhido. Evite publicar diagnosticos privados. Um proxy VPN, transparente ou de operadora ainda pode existir; `--noproxy '*'` nao garante canal fisico direto.
- NTP apenas ajusta/verifica horario. Nao e transporte de paginas ou prova da rota. DEX e formato da VM Android; GPU e NUMA nao fornecem vantagem para verificar identidade DNS/TLS.

## Sinais, falsificadores e decisao

| Sinal | Relevancia | Contraexemplo / limite |
| --- | --- | --- |
| Private DNS desativado, opportunistic ou indeterminado | alerta de configuracao | opcoes Android podem estar indisponiveis; observar nao equivale a provar uso efetivo |
| Proxy e interface tun/ppp/wg visiveis | investigar configuracao inesperada | VPN ou proxy corporativo legitimo; ausencia de sinal nao exclui tunel |
| IP via sistema diferente de IP via DoH | divergencia para revisao | CDN, anycast e balanceamento mudam IP normalmente; nao e malware confirmado |
| Falha da verificacao TLS do curl | nao confiar nessa tentativa | falta de CA, relogio errado, filtro ou falha da rede; nao determina a causa |
| Conexao a 1.1.1.1 por `--resolve` | evita resolucao de DNS *desse destino* | nao evita NAT, VPN, middlebox, roteamento IP; preserva hostname para TLS |
| Ping, DNS/connect/TLS/total | variacao de desempenho | RTT nao determina distancia fisica e nao prova interceptacao |
| NTP configurado | indicio de mecanismo de hora | nao prova hora correta, integridade de rede ou DNS |

O BrowserRaf atual usa 8.8.8.8:53 UDP sem DNSSEC/criptografia e **ainda recusa HTTPS**. Integrar protecao exige Android `ConnectivityManager/LinkProperties/DnsResolver` ou biblioteca TLS auditada, estrategia de CA/hostname e politica de resolucao; nao basta ativar `br_tls.h`. O codigo produtivo `internal/` nao foi alterado nesta fase.

## Gates / aceite

- G0: testes hermeticos provam que `--offline` nao chama `curl` e que opcoes desconhecidas falham.
- G1: `--active` usa apenas HTTPS sem `-k/-L`, confere explicitamente o hostname e nao troca ip direto por HTTP em claro.
- G2: erro TLS simulado gera `FALHA` e `TOKEN_VAZIO_UNMEASURED`, nunca laudo de malware.
- G3: divergent DNS e classificado **nao conclusivo**; sucesso TLS tambem nao prova ausencia de malware.
- G4: Android fisico, conectividade real, rede celular/Wi-Fi/VPN, permissao de `settings` e recibo SHA ainda **NOT_RUN**.
- G5: monitoramento fora do navegador so com modelo separado e consentido, possivelmente `VpnService`; nao interceptar nem quebrar TLS de outros aplicativos, nem instalar CAs sem autorizacao.
- G6: auditar gates, CI exact-head, revisao independente e privacidade antes de promover o produto. CI de mocks nao equivale a teste em dispositivo.

**ROLLBACK**: branch/PR draft separado; nao toca em `internal/`, compilador, APK nem alteracoes do sistema. Fechar PR antes do merge; apos merge, revert revisado.

**PV**: SOURCE=BrowserRaf/main e docs observados; ARTIFACT=tool/test/docs/workflow; EXECUTION=teste de shell com mocks locais; EVIDENCE=PASS_SCOPED (nao testa rede real); CLAIM=prototipo diagnostico limitado. 
