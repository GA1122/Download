static int quota_cb(const mbentry_t *mbentry, void *rock)
{
    const char *servername = (const char *)rock;
    int r;

    if (strcmp(servername, mbentry->server)) {
         
        r = IMAP_NOT_SINGULAR_ROOT;
    } else {
        r = PROXY_OK;
    }

    return r;
}