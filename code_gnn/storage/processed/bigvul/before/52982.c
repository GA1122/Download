static ssize_t ui_read(struct file *filp, char __user *buf, size_t count,
		       loff_t *f_pos)
{
	struct hfi1_devdata *dd = filp->private_data;
	void __iomem *base = dd->kregbase;
	unsigned long total, csr_off,
		barlen = (dd->kregend - dd->kregbase);
	u64 data;

	 
	if ((count % 8) != 0)
		return -EINVAL;
	 
	if ((*f_pos % 8) != 0)
		return -EINVAL;
	 
	if ((unsigned long)buf % 8 != 0)
		return -EINVAL;
	 
	if (*f_pos + count > (barlen + DC8051_DATA_MEM_SIZE))
		return -EINVAL;
	 
	if (*f_pos < barlen)
		base += *f_pos;
	csr_off = *f_pos;
	for (total = 0; total < count; total += 8, csr_off += 8) {
		 
		if (is_lcb_offset(csr_off)) {
			if (read_lcb_csr(dd, csr_off, (u64 *)&data))
				break;  
		}
		 
		else if (csr_off == ASIC_GPIO_CLEAR ||
			 csr_off == ASIC_GPIO_FORCE ||
			 csr_off == ASIC_QSFP1_CLEAR ||
			 csr_off == ASIC_QSFP1_FORCE ||
			 csr_off == ASIC_QSFP2_CLEAR ||
			 csr_off == ASIC_QSFP2_FORCE)
			data = 0;
		else if (csr_off >= barlen) {
			 
			if (read_8051_data(dd,
					   (u32)(csr_off - barlen),
					   sizeof(data), &data))
				break;  
		} else
			data = readq(base + total);
		if (put_user(data, (unsigned long __user *)(buf + total)))
			break;
	}
	*f_pos += total;
	return total;
}
