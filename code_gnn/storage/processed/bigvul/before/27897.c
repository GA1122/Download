void fuse_release_common(struct file *file, int opcode)
{
	struct fuse_file *ff;
	struct fuse_req *req;

	ff = file->private_data;
	if (unlikely(!ff))
		return;

	req = ff->reserved_req;
	fuse_prepare_release(ff, file->f_flags, opcode);

	 
	path_get(&file->f_path);
	req->misc.release.path = file->f_path;

	 
	fuse_file_put(ff);
}
