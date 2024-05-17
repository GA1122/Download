static int handle_setattr(struct fuse* fuse, struct fuse_handler* handler,
 const struct fuse_in_header *hdr, const struct fuse_setattr_in *req)
{
 struct node* node;
 char path[PATH_MAX];
 struct timespec times[2];

    pthread_mutex_lock(&fuse->global->lock);
    node = lookup_node_and_path_by_id_locked(fuse, hdr->nodeid, path, sizeof(path));
    TRACE("[%d] SETATTR fh=%"PRIx64" valid=%x @ %"PRIx64" (%s)\n", handler->token,
            req->fh, req->valid, hdr->nodeid, node ? node->name : "?");
    pthread_mutex_unlock(&fuse->global->lock);

 if (!node) {
 return -ENOENT;
 }

 if (!(req->valid & FATTR_FH) &&
 !check_caller_access_to_node(fuse, hdr, node, W_OK)) {
 return -EACCES;
 }

  

 if ((req->valid & FATTR_SIZE) && truncate64(path, req->size) < 0) {
 return -errno;
 }

  
 if (req->valid & (FATTR_ATIME | FATTR_MTIME)) {
        times[0].tv_nsec = UTIME_OMIT;
        times[1].tv_nsec = UTIME_OMIT;
 if (req->valid & FATTR_ATIME) {
 if (req->valid & FATTR_ATIME_NOW) {
              times[0].tv_nsec = UTIME_NOW;
 } else {
              times[0].tv_sec = req->atime;
              times[0].tv_nsec = req->atimensec;
 }
 }
 if (req->valid & FATTR_MTIME) {
 if (req->valid & FATTR_MTIME_NOW) {
              times[1].tv_nsec = UTIME_NOW;
 } else {
              times[1].tv_sec = req->mtime;
              times[1].tv_nsec = req->mtimensec;
 }
 }
        TRACE("[%d] Calling utimensat on %s with atime %ld, mtime=%ld\n",
                handler->token, path, times[0].tv_sec, times[1].tv_sec);
 if (utimensat(-1, path, times, 0) < 0) {
 return -errno;
 }
 }
 return fuse_reply_attr(fuse, hdr->unique, node, path);
}
