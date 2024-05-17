ssize_t fuse_simple_request(struct fuse_conn *fc, struct fuse_args *args)
{
	struct fuse_req *req;
	ssize_t ret;

	req = fuse_get_req(fc, 0);
	if (IS_ERR(req))
		return PTR_ERR(req);

	 
	fuse_adjust_compat(fc, args);

	req->in.h.opcode = args->in.h.opcode;
	req->in.h.nodeid = args->in.h.nodeid;
	req->in.numargs = args->in.numargs;
	memcpy(req->in.args, args->in.args,
	       args->in.numargs * sizeof(struct fuse_in_arg));
	req->out.argvar = args->out.argvar;
	req->out.numargs = args->out.numargs;
	memcpy(req->out.args, args->out.args,
	       args->out.numargs * sizeof(struct fuse_arg));
	fuse_request_send(fc, req);
	ret = req->out.h.error;
	if (!ret && args->out.argvar) {
		BUG_ON(args->out.numargs != 1);
		ret = req->out.args[0].size;
	}
	fuse_put_request(fc, req);

	return ret;
}