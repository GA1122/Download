static int on_http_header_value(http_parser* parser, const char* at, size_t length)
{
    struct clt_info *info = parser->data;
    info->value_set = true;
    if (info->value == NULL) {
        info->value = sdsnewlen(at, length);
    } else {
        info->value = sdscpylen(info->value, at, length);
    }

    if (info->field_set && info->value_set) {
        http_request_set_header(info->request, info->field, info->value);
        info->field_set = false;
        info->value_set = false;
    }

    return 0;
}
