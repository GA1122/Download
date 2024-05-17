static int on_http_header_field(http_parser* parser, const char* at, size_t length)
{
    struct clt_info *info = parser->data;
    info->field_set = true;
    if (info->field == NULL) {
        info->field = sdsnewlen(at, length);
    } else {
        info->field = sdscpylen(info->field, at, length);
    }

    return 0;
}
