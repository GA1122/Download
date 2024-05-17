static int handle_flush(struct fuse* fuse, struct fuse_handler* handler,
 const struct fuse_in_header* hdr)
{
    TRACE("[%d] FLUSH\n", handler->token);
 return 0;
}
