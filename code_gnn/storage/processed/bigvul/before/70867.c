static int http_shutdown(URLContext *h, int flags)
{
    int ret = 0;
    char footer[] = "0\r\n\r\n";
    HTTPContext *s = h->priv_data;

     
    if (((flags & AVIO_FLAG_WRITE) && s->chunked_post) ||
        ((flags & AVIO_FLAG_READ) && s->chunked_post && s->listen)) {
        ret = ffurl_write(s->hd, footer, sizeof(footer) - 1);
        ret = ret > 0 ? 0 : ret;
        s->end_chunked_post = 1;
    }

    return ret;
}