static void fuse_notify_delete(struct fuse* fuse, const __u64 parent,
 const __u64 child, const char* name) {
 struct fuse_out_header hdr;
 struct fuse_notify_delete_out data;
 struct iovec vec[3];
 size_t namelen = strlen(name);
 int res;

    hdr.len = sizeof(hdr) + sizeof(data) + namelen + 1;
    hdr.error = FUSE_NOTIFY_DELETE;
    hdr.unique = 0;

    data.parent = parent;
    data.child = child;
    data.namelen = namelen;
    data.padding = 0;

    vec[0].iov_base = &hdr;
    vec[0].iov_len = sizeof(hdr);
    vec[1].iov_base = &data;
    vec[1].iov_len = sizeof(data);
    vec[2].iov_base = (void*) name;
    vec[2].iov_len = namelen + 1;

    res = writev(fuse->fd, vec, 3);
  
 if (res < 0 && errno != ENOENT) {
        ERROR("*** NOTIFY FAILED *** %d\n", errno);
 }
}
