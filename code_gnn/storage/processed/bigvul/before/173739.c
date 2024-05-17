static void handle_fuse_requests(struct fuse_handler* handler)
{
 struct fuse* fuse = handler->fuse;
 for (;;) {
 ssize_t len = TEMP_FAILURE_RETRY(read(fuse->fd,
                handler->request_buffer, sizeof(handler->request_buffer)));
 if (len < 0) {
 if (errno == ENODEV) {
                ERROR("[%d] someone stole our marbles!\n", handler->token);
                exit(2);
 }
            ERROR("[%d] handle_fuse_requests: errno=%d\n", handler->token, errno);
 continue;
 }

 if ((size_t)len < sizeof(struct fuse_in_header)) {
            ERROR("[%d] request too short: len=%zu\n", handler->token, (size_t)len);
 continue;
 }

 const struct fuse_in_header *hdr = (void*)handler->request_buffer;
 if (hdr->len != (size_t)len) {
            ERROR("[%d] malformed header: len=%zu, hdr->len=%u\n",
                    handler->token, (size_t)len, hdr->len);
 continue;
 }

 const void *data = handler->request_buffer + sizeof(struct fuse_in_header);
 size_t data_len = len - sizeof(struct fuse_in_header);
        __u64 unique = hdr->unique;
 int res = handle_fuse_request(fuse, handler, hdr, data, data_len);

  

 if (res != NO_STATUS) {
 if (res) {
                TRACE("[%d] ERROR %d\n", handler->token, res);
 }
            fuse_status(fuse, unique, res);
 }
 }
}
