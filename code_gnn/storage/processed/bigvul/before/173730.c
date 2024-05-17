static void fuse_reply(struct fuse *fuse, __u64 unique, void *data, int len)
{
 struct fuse_out_header hdr;
 struct iovec vec[2];
 int res;

    hdr.len = len + sizeof(hdr);
    hdr.error = 0;
    hdr.unique = unique;

    vec[0].iov_base = &hdr;
    vec[0].iov_len = sizeof(hdr);
    vec[1].iov_base = data;
    vec[1].iov_len = len;

    res = writev(fuse->fd, vec, 2);
 if (res < 0) {
        ERROR("*** REPLY FAILED *** %d\n", errno);
 }
}
