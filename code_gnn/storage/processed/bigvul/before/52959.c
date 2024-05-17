static int assign_ctxt(struct file *fp, struct hfi1_user_info *uinfo)
{
	int i_minor, ret = 0;
	unsigned swmajor, swminor, alg = HFI1_ALG_ACROSS;

	swmajor = uinfo->userversion >> 16;
	if (swmajor != HFI1_USER_SWMAJOR) {
		ret = -ENODEV;
		goto done;
	}

	swminor = uinfo->userversion & 0xffff;

	if (uinfo->hfi1_alg < HFI1_ALG_COUNT)
		alg = uinfo->hfi1_alg;

	mutex_lock(&hfi1_mutex);
	 
	if (uinfo->subctxt_cnt) {
		struct hfi1_filedata *fd = fp->private_data;

		ret = find_shared_ctxt(fp, uinfo);
		if (ret < 0)
			goto done_unlock;
		if (ret)
			fd->rec_cpu_num = hfi1_get_proc_affinity(
				fd->uctxt->dd, fd->uctxt->numa_id);
	}

	 
	if (!ret) {
		i_minor = iminor(file_inode(fp)) - HFI1_USER_MINOR_BASE;
		ret = get_user_context(fp, uinfo, i_minor - 1, alg);
	}
done_unlock:
	mutex_unlock(&hfi1_mutex);
done:
	return ret;
}