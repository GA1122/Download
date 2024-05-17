static int handle_opendir(struct fuse* fuse, struct fuse_handler* handler,
 const struct fuse_in_header* hdr, const struct fuse_open_in* req)
{
 struct node* node;
 char path[PATH_MAX];
 struct fuse_open_out out;
 struct dirhandle *h;

    pthread_mutex_lock(&fuse->global->lock);
    node = lookup_node_and_path_by_id_locked(fuse, hdr->nodeid, path, sizeof(path));
    TRACE("[%d] OPENDIR @ %"PRIx64" (%s)\n", handler->token,
            hdr->nodeid, node ? node->name : "?");
    pthread_mutex_unlock(&fuse->global->lock);

 if (!node) {
 return -ENOENT;
 }
 if (!check_caller_access_to_node(fuse, hdr, node, R_OK)) {
 return -EACCES;
 }
    h = malloc(sizeof(*h));
 if (!h) {
 return -ENOMEM;
 }
    TRACE("[%d] OPENDIR %s\n", handler->token, path);
    h->d = opendir(path);
 if (!h->d) {
        free(h);
 return -errno;
 }
    out.fh = ptr_to_id(h);
    out.open_flags = 0;
    out.padding = 0;
    fuse_reply(fuse, hdr->unique, &out, sizeof(out));
 return NO_STATUS;
}
