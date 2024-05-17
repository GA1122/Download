static ssize_t ucma_init_qp_attr(struct ucma_file *file,
				 const char __user *inbuf,
				 int in_len, int out_len)
{
	struct rdma_ucm_init_qp_attr cmd;
	struct ib_uverbs_qp_attr resp;
	struct ucma_context *ctx;
	struct ib_qp_attr qp_attr;
	int ret;

	if (out_len < sizeof(resp))
		return -ENOSPC;

	if (copy_from_user(&cmd, inbuf, sizeof(cmd)))
		return -EFAULT;

	ctx = ucma_get_ctx(file, cmd.id);
	if (IS_ERR(ctx))
		return PTR_ERR(ctx);

	resp.qp_attr_mask = 0;
	memset(&qp_attr, 0, sizeof qp_attr);
	qp_attr.qp_state = cmd.qp_state;
	ret = rdma_init_qp_attr(ctx->cm_id, &qp_attr, &resp.qp_attr_mask);
	if (ret)
		goto out;

	ib_copy_qp_attr_to_user(&resp, &qp_attr);
	if (copy_to_user((void __user *)(unsigned long)cmd.response,
			 &resp, sizeof(resp)))
		ret = -EFAULT;

out:
	ucma_put_ctx(ctx);
	return ret;
}