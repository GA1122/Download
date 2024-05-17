static unsigned int qib_poll(struct file *fp, struct poll_table_struct *pt)
{
	struct qib_ctxtdata *rcd;
	unsigned pollflag;

	rcd = ctxt_fp(fp);
	if (!rcd)
		pollflag = POLLERR;
	else if (rcd->poll_type == QIB_POLL_TYPE_URGENT)
		pollflag = qib_poll_urgent(rcd, fp, pt);
	else  if (rcd->poll_type == QIB_POLL_TYPE_ANYRCV)
		pollflag = qib_poll_next(rcd, fp, pt);
	else  
		pollflag = POLLERR;

	return pollflag;
}