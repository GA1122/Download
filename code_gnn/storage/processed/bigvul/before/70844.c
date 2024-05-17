static int check_http_code(URLContext *h, int http_code, const char *end)
{
    HTTPContext *s = h->priv_data;
     
    if (http_code >= 400 && http_code < 600 &&
        (http_code != 401 || s->auth_state.auth_type != HTTP_AUTH_NONE) &&
        (http_code != 407 || s->proxy_auth_state.auth_type != HTTP_AUTH_NONE)) {
        end += strspn(end, SPACE_CHARS);
        av_log(h, AV_LOG_WARNING, "HTTP error %d %s\n", http_code, end);
        return ff_http_averror(http_code, AVERROR(EIO));
    }
    return 0;
}
