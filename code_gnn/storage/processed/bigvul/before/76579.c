static int on_http_message_begin(http_parser* parser)
{
    struct clt_info *info = parser->data;
    if (info->request)
        http_request_release(info->request);
    info->request = http_request_new();
    if (info->request == NULL) {
        return -__LINE__;
    }

    return 0;
}
