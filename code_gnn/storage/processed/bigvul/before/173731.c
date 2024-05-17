static int fuse_reply_attr(struct fuse* fuse, __u64 unique, const struct node* node,
 const char* path)
{
 struct fuse_attr_out out;
 struct stat s;

 if (lstat(path, &s) < 0) {
 return -errno;
 }
    memset(&out, 0, sizeof(out));
    attr_from_stat(fuse, &out.attr, &s, node);
    out.attr_valid = 10;
    fuse_reply(fuse, unique, &out, sizeof(out));
 return NO_STATUS;
}
