static ssize_t ui_write(struct file *filp, const char __user *buf,
			size_t count, loff_t *f_pos)
{
	struct hfi1_devdata *dd = filp->private_data;
	void __iomem *base;
	unsigned long total, data, csr_off;
	int in_lcb;

	 
	if ((count % 8) != 0)
		return -EINVAL;
	 
	if ((*f_pos % 8) != 0)
		return -EINVAL;
	 
	if ((unsigned long)buf % 8 != 0)
		return -EINVAL;
	 
	if (*f_pos + count > dd->kregend - dd->kregbase)
		return -EINVAL;

	base = (void __iomem *)dd->kregbase + *f_pos;
	csr_off = *f_pos;
	in_lcb = 0;
	for (total = 0; total < count; total += 8, csr_off += 8) {
		if (get_user(data, (unsigned long __user *)(buf + total)))
			break;
		 
		if (is_lcb_offset(csr_off)) {
			if (!in_lcb) {
				int ret = acquire_lcb_access(dd, 1);

				if (ret)
					break;
				in_lcb = 1;
			}
		} else {
			if (in_lcb) {
				release_lcb_access(dd, 1);
				in_lcb = 0;
			}
		}
		writeq(data, base + total);
	}
	if (in_lcb)
		release_lcb_access(dd, 1);
	*f_pos += total;
	return total;
}
