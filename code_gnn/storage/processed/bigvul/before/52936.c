static int qib_do_user_init(struct file *fp,
			    const struct qib_user_info *uinfo)
{
	int ret;
	struct qib_ctxtdata *rcd = ctxt_fp(fp);
	struct qib_devdata *dd;
	unsigned uctxt;

	 
	if (subctxt_fp(fp)) {
		ret = wait_event_interruptible(rcd->wait,
			!test_bit(QIB_CTXT_MASTER_UNINIT, &rcd->flag));
		goto bail;
	}

	dd = rcd->dd;

	 
	uctxt = rcd->ctxt - dd->first_user_ctxt;
	if (uctxt < dd->ctxts_extrabuf) {
		rcd->piocnt = dd->pbufsctxt + 1;
		rcd->pio_base = rcd->piocnt * uctxt;
	} else {
		rcd->piocnt = dd->pbufsctxt;
		rcd->pio_base = rcd->piocnt * uctxt +
			dd->ctxts_extrabuf;
	}

	 
	if ((rcd->pio_base + rcd->piocnt) > dd->piobcnt2k) {
		if (rcd->pio_base >= dd->piobcnt2k) {
			qib_dev_err(dd,
				    "%u:ctxt%u: no 2KB buffers available\n",
				    dd->unit, rcd->ctxt);
			ret = -ENOBUFS;
			goto bail;
		}
		rcd->piocnt = dd->piobcnt2k - rcd->pio_base;
		qib_dev_err(dd, "Ctxt%u: would use 4KB bufs, using %u\n",
			    rcd->ctxt, rcd->piocnt);
	}

	rcd->piobufs = dd->pio2k_bufbase + rcd->pio_base * dd->palign;
	qib_chg_pioavailkernel(dd, rcd->pio_base, rcd->piocnt,
			       TXCHK_CHG_TYPE_USER, rcd);
	 
	dd->f_sendctrl(dd->pport, QIB_SENDCTRL_AVAIL_BLIP);

	 
	ret = qib_create_rcvhdrq(dd, rcd);
	if (!ret)
		ret = qib_setup_eagerbufs(rcd);
	if (ret)
		goto bail_pio;

	rcd->tidcursor = 0;  

	 
	rcd->urgent = 0;
	rcd->urgent_poll = 0;

	 
	if (rcd->rcvhdrtail_kvaddr)
		qib_clear_rcvhdrtail(rcd);

	dd->f_rcvctrl(rcd->ppd, QIB_RCVCTRL_CTXT_ENB | QIB_RCVCTRL_TIDFLOW_ENB,
		      rcd->ctxt);

	 
	if (rcd->subctxt_cnt) {
		clear_bit(QIB_CTXT_MASTER_UNINIT, &rcd->flag);
		wake_up(&rcd->wait);
	}
	return 0;

bail_pio:
	qib_chg_pioavailkernel(dd, rcd->pio_base, rcd->piocnt,
			       TXCHK_CHG_TYPE_KERN, rcd);
bail:
	return ret;
}
