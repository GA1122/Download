static int qib_ctxt_info(struct file *fp, struct qib_ctxt_info __user *uinfo)
{
	struct qib_ctxt_info info;
	int ret;
	size_t sz;
	struct qib_ctxtdata *rcd = ctxt_fp(fp);
	struct qib_filedata *fd;

	fd = fp->private_data;

	info.num_active = qib_count_active_units();
	info.unit = rcd->dd->unit;
	info.port = rcd->ppd->port;
	info.ctxt = rcd->ctxt;
	info.subctxt =  subctxt_fp(fp);
	 
	info.num_ctxts = rcd->dd->cfgctxts - rcd->dd->first_user_ctxt;
	info.num_subctxts = rcd->subctxt_cnt;
	info.rec_cpu = fd->rec_cpu_num;
	sz = sizeof(info);

	if (copy_to_user(uinfo, &info, sz)) {
		ret = -EFAULT;
		goto bail;
	}
	ret = 0;

bail:
	return ret;
}