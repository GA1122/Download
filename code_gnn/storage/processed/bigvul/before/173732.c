static int fuse_reply_entry(struct fuse* fuse, __u64 unique,
 struct node* parent, const char* name, const char* actual_name,
 const char* path)
{
 struct node* node;
 struct fuse_entry_out out;
 struct stat s;

 if (lstat(path, &s) < 0) {
 return -errno;
 }

    pthread_mutex_lock(&fuse->global->lock);
    node = acquire_or_create_child_locked(fuse, parent, name, actual_name);
 if (!node) {
        pthread_mutex_unlock(&fuse->global->lock);
 return -ENOMEM;
 }
    memset(&out, 0, sizeof(out));
    attr_from_stat(fuse, &out.attr, &s, node);
    out.attr_valid = 10;
    out.entry_valid = 10;
    out.nodeid = node->nid;
    out.generation = node->gen;
    pthread_mutex_unlock(&fuse->global->lock);
    fuse_reply(fuse, unique, &out, sizeof(out));
 return NO_STATUS;
}
