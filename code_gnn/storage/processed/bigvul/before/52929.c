static int qib_close(struct inode *in, struct file *fp)
{
	int ret = 0;
	struct qib_filedata *fd;
	struct qib_ctxtdata *rcd;
	struct qib_devdata *dd;
	unsigned long flags;
	unsigned ctxt;
	pid_t pid;

	mutex_lock(&qib_mutex);

	fd = fp->private_data;
	fp->private_data = NULL;
	rcd = fd->rcd;
	if (!rcd) {
		mutex_unlock(&qib_mutex);
		goto bail;
	}

	dd = rcd->dd;

	 
	qib_flush_wc();

	 
	if (fd->pq) {
		qib_user_sdma_queue_drain(rcd->ppd, fd->pq);
		qib_user_sdma_queue_destroy(fd->pq);
	}

	if (fd->rec_cpu_num != -1)
		__clear_bit(fd->rec_cpu_num, qib_cpulist);

	if (--rcd->cnt) {
		 
		rcd->active_slaves &= ~(1 << fd->subctxt);
		rcd->subpid[fd->subctxt] = 0;
		mutex_unlock(&qib_mutex);
		goto bail;
	}

	 
	spin_lock_irqsave(&dd->uctxt_lock, flags);
	ctxt = rcd->ctxt;
	dd->rcd[ctxt] = NULL;
	pid = rcd->pid;
	rcd->pid = 0;
	spin_unlock_irqrestore(&dd->uctxt_lock, flags);

	if (rcd->rcvwait_to || rcd->piowait_to ||
	    rcd->rcvnowait || rcd->pionowait) {
		rcd->rcvwait_to = 0;
		rcd->piowait_to = 0;
		rcd->rcvnowait = 0;
		rcd->pionowait = 0;
	}
	if (rcd->flag)
		rcd->flag = 0;

	if (dd->kregbase) {
		 
		dd->f_rcvctrl(rcd->ppd, QIB_RCVCTRL_CTXT_DIS |
				  QIB_RCVCTRL_INTRAVAIL_DIS, ctxt);

		 
		qib_clean_part_key(rcd, dd);
		qib_disarm_piobufs(dd, rcd->pio_base, rcd->piocnt);
		qib_chg_pioavailkernel(dd, rcd->pio_base,
				       rcd->piocnt, TXCHK_CHG_TYPE_KERN, NULL);

		dd->f_clear_tids(dd, rcd);

		if (dd->pageshadow)
			unlock_expected_tids(rcd);
		qib_stats.sps_ctxts--;
		dd->freectxts++;
	}

	mutex_unlock(&qib_mutex);
	qib_free_ctxtdata(dd, rcd);  

bail:
	kfree(fd);
	return ret;
}
