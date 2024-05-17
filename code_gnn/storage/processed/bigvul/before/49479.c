u8 *hid_alloc_report_buf(struct hid_report *report, gfp_t flags)
{
	 

	int len = hid_report_len(report) + 7;

	return kmalloc(len, flags);
}
