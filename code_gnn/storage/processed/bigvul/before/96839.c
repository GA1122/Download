static void put_reserved_req(struct fuse_conn *fc, struct fuse_req *req)
{
	struct file *file = req->stolen_file;
	struct fuse_inode *fi = get_fuse_inode(file_inode(file));
	struct fuse_file *ff = file->private_data;

	WARN_ON(req->max_pages);
	spin_lock(&fi->lock);
	memset(req, 0, sizeof(*req));
	fuse_request_init(req, NULL, NULL, 0);
	BUG_ON(ff->reserved_req);
	ff->reserved_req = req;
	wake_up_all(&fc->reserved_req_waitq);
	spin_unlock(&fi->lock);
	fput(file);
}