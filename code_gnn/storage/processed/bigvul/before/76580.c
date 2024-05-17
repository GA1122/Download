static int on_http_url(http_parser* parser, const char* at, size_t length)
{
    struct clt_info *info = parser->data;
    if (info->request->url)
        sdsfree(info->request->url);
    info->request->url = sdsnewlen(at, length);

    return 0;
}
