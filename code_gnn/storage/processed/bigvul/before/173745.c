static int handle_open(struct fuse* fuse, struct fuse_handler* handler,
 const struct fuse_in_header* hdr, const struct fuse_open_in* req)
{
 struct node* node;
 char path[PATH_MAX];
 struct fuse_open_out out;
 struct handle *h;

    pthread_mutex_lock(&fuse->global->lock);
    node = lookup_node_and_path_by_id_locked(fuse, hdr->nodeid, path, sizeof(path));
    TRACE("[%d] OPEN 0%o @ %"PRIx64" (%s)\n", handler->token,
            req->flags, hdr->nodeid, node ? node->name : "?");
    pthread_mutex_unlock(&fuse->global->lock);

 if (!node) {
 return -ENOENT;
 }
 if (!check_caller_access_to_node(fuse, hdr, node,
            open_flags_to_access_mode(req->flags))) {
 return -EACCES;
 }
    h = malloc(sizeof(*h));
 if (!h) {
 return -ENOMEM;
 }
    TRACE("[%d] OPEN %s\n", handler->token, path);
    h->fd = open(path, req->flags);
 if (h->fd < 0) {
        free(h);
 return -errno;
 }
    out.fh = ptr_to_id(h);
    out.open_flags = 0;
    out.padding = 0;
    fuse_reply(fuse, hdr->unique, &out, sizeof(out));
 return NO_STATUS;
}
