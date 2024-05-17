static int handle_init(struct fuse* fuse, struct fuse_handler* handler,
 const struct fuse_in_header* hdr, const struct fuse_init_in* req)
{
 struct fuse_init_out out;
 size_t fuse_struct_size;

    TRACE("[%d] INIT ver=%d.%d maxread=%d flags=%x\n",
            handler->token, req->major, req->minor, req->max_readahead, req->flags);

  
 if (req->major != FUSE_KERNEL_VERSION || req->minor < 6) {
        ERROR("Fuse kernel version mismatch: Kernel version %d.%d, Expected at least %d.6",
              req->major, req->minor, FUSE_KERNEL_VERSION);
 return -1;
 }

  
    out.minor = MIN(req->minor, 15);
    fuse_struct_size = sizeof(out);
#if defined(FUSE_COMPAT_22_INIT_OUT_SIZE)
  

  
 if (req->minor <= 22) {
        fuse_struct_size = FUSE_COMPAT_22_INIT_OUT_SIZE;
 }
#endif

    out.major = FUSE_KERNEL_VERSION;
    out.max_readahead = req->max_readahead;
    out.flags = FUSE_ATOMIC_O_TRUNC | FUSE_BIG_WRITES;
    out.max_background = 32;
    out.congestion_threshold = 32;
    out.max_write = MAX_WRITE;
    fuse_reply(fuse, hdr->unique, &out, fuse_struct_size);
 return NO_STATUS;
}
