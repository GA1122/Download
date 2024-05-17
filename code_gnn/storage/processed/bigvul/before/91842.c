void log_access(request * req)
{
    if (!access_log_name)
        return;

    if (virtualhost) {
        printf("%s ", req->local_ip_addr);
    } else if (vhost_root) {
        printf("%s ", (req->host ? req->host : "(null)"));
    }
    print_remote_ip (req, stdout);
    printf(" - - %s\"%s\" %d %zu \"%s\" \"%s\"\n",
           get_commonlog_time(),
           req->logline ? req->logline : "-",
           req->response_status,
           req->bytes_written,
           (req->header_referer ? req->header_referer : "-"),
           (req->header_user_agent ? req->header_user_agent : "-"));
}
