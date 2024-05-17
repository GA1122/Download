static int find_shared_ctxt(struct file *fp,
			    const struct qib_user_info *uinfo)
{
	int devmax, ndev, i;
	int ret = 0;

	devmax = qib_count_units(NULL, NULL);

	for (ndev = 0; ndev < devmax; ndev++) {
		struct qib_devdata *dd = qib_lookup(ndev);

		 
		if (!(dd && (dd->flags & QIB_PRESENT) && dd->kregbase))
			continue;
		for (i = dd->first_user_ctxt; i < dd->cfgctxts; i++) {
			struct qib_ctxtdata *rcd = dd->rcd[i];

			 
			if (!rcd || !rcd->cnt)
				continue;
			 
			if (rcd->subctxt_id != uinfo->spu_subctxt_id)
				continue;
			 
			if (rcd->subctxt_cnt != uinfo->spu_subctxt_cnt ||
			    rcd->userversion != uinfo->spu_userversion ||
			    rcd->cnt >= rcd->subctxt_cnt) {
				ret = -EINVAL;
				goto done;
			}
			ctxt_fp(fp) = rcd;
			subctxt_fp(fp) = rcd->cnt++;
			rcd->subpid[subctxt_fp(fp)] = current->pid;
			tidcursor_fp(fp) = 0;
			rcd->active_slaves |= 1 << subctxt_fp(fp);
			ret = 1;
			goto done;
		}
	}

done:
	return ret;
}
