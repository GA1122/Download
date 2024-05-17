static int handle_lookup(struct fuse* fuse, struct fuse_handler* handler,
 const struct fuse_in_header *hdr, const char* name)
{
 struct node* parent_node;
 char parent_path[PATH_MAX];
 char child_path[PATH_MAX];
 const char* actual_name;

    pthread_mutex_lock(&fuse->global->lock);
    parent_node = lookup_node_and_path_by_id_locked(fuse, hdr->nodeid,
            parent_path, sizeof(parent_path));
    TRACE("[%d] LOOKUP %s @ %"PRIx64" (%s)\n", handler->token, name, hdr->nodeid,
        parent_node ? parent_node->name : "?");
    pthread_mutex_unlock(&fuse->global->lock);

 if (!parent_node || !(actual_name = find_file_within(parent_path, name,
            child_path, sizeof(child_path), 1))) {
 return -ENOENT;
 }
 if (!check_caller_access_to_name(fuse, hdr, parent_node, name, R_OK)) {
 return -EACCES;
 }

 return fuse_reply_entry(fuse, hdr->unique, parent_node, name, actual_name, child_path);
}
