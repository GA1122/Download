static int handle_getattr(struct fuse* fuse, struct fuse_handler* handler,
 const struct fuse_in_header *hdr, const struct fuse_getattr_in *req)
{
 struct node* node;
 char path[PATH_MAX];

    pthread_mutex_lock(&fuse->global->lock);
    node = lookup_node_and_path_by_id_locked(fuse, hdr->nodeid, path, sizeof(path));
    TRACE("[%d] GETATTR flags=%x fh=%"PRIx64" @ %"PRIx64" (%s)\n", handler->token,
            req->getattr_flags, req->fh, hdr->nodeid, node ? node->name : "?");
    pthread_mutex_unlock(&fuse->global->lock);

 if (!node) {
 return -ENOENT;
 }
 if (!check_caller_access_to_node(fuse, hdr, node, R_OK)) {
 return -EACCES;
 }

 return fuse_reply_attr(fuse, hdr->unique, node, path);
}
