static int http_close(URLContext *h)
{
    int ret = 0;
    HTTPContext *s = h->priv_data;

#if CONFIG_ZLIB
    inflateEnd(&s->inflate_stream);
    av_freep(&s->inflate_buffer);
#endif  

    if (!s->end_chunked_post)
         
        ret = http_shutdown(h, h->flags);

    if (s->hd)
        ffurl_closep(&s->hd);
    av_dict_free(&s->chained_options);
    return ret;
}
