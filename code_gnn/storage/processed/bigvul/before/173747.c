static int handle_read(struct fuse* fuse, struct fuse_handler* handler,
 const struct fuse_in_header* hdr, const struct fuse_read_in* req)
{
 struct handle *h = id_to_ptr(req->fh);
    __u64 unique = hdr->unique;
    __u32 size = req->size;
    __u64 offset = req->offset;
 int res;
    __u8 *read_buffer = (__u8 *) ((uintptr_t)(handler->read_buffer + PAGESIZE) & ~((uintptr_t)PAGESIZE-1));

  

    TRACE("[%d] READ %p(%d) %u@%"PRIu64"\n", handler->token,
            h, h->fd, size, (uint64_t) offset);
 if (size > MAX_READ) {
 return -EINVAL;
 }
    res = pread64(h->fd, read_buffer, size, offset);
 if (res < 0) {
 return -errno;
 }
    fuse_reply(fuse, unique, read_buffer, res);
 return NO_STATUS;
}
