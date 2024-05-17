static int http_listen(URLContext *h, const char *uri, int flags,
                       AVDictionary **options) {
    HTTPContext *s = h->priv_data;
    int ret;
    char hostname[1024], proto[10];
    char lower_url[100];
    const char *lower_proto = "tcp";
    int port;
    av_url_split(proto, sizeof(proto), NULL, 0, hostname, sizeof(hostname), &port,
                 NULL, 0, uri);
    if (!strcmp(proto, "https"))
        lower_proto = "tls";
    ff_url_join(lower_url, sizeof(lower_url), lower_proto, NULL, hostname, port,
                NULL);
    if ((ret = av_dict_set_int(options, "listen", s->listen, 0)) < 0)
        goto fail;
    if ((ret = ffurl_open_whitelist(&s->hd, lower_url, AVIO_FLAG_READ_WRITE,
                                    &h->interrupt_callback, options,
                                    h->protocol_whitelist, h->protocol_blacklist, h
                                   )) < 0)
        goto fail;
    s->handshake_step = LOWER_PROTO;
    if (s->listen == HTTP_SINGLE) {  
        s->reply_code = 200;
        while ((ret = http_handshake(h)) > 0);
    }
fail:
    av_dict_free(&s->chained_options);
    return ret;
}