static ssize_t ucma_query(struct ucma_file *file,
			  const char __user *inbuf,
			  int in_len, int out_len)
{
	struct rdma_ucm_query cmd;
	struct ucma_context *ctx;
	void __user *response;
	int ret;

	if (copy_from_user(&cmd, inbuf, sizeof(cmd)))
		return -EFAULT;

	response = (void __user *)(unsigned long) cmd.response;
	ctx = ucma_get_ctx(file, cmd.id);
	if (IS_ERR(ctx))
		return PTR_ERR(ctx);

	switch (cmd.option) {
	case RDMA_USER_CM_QUERY_ADDR:
		ret = ucma_query_addr(ctx, response, out_len);
		break;
	case RDMA_USER_CM_QUERY_PATH:
		ret = ucma_query_path(ctx, response, out_len);
		break;
	case RDMA_USER_CM_QUERY_GID:
		ret = ucma_query_gid(ctx, response, out_len);
		break;
	default:
		ret = -ENOSYS;
		break;
	}

	ucma_put_ctx(ctx);
	return ret;
}