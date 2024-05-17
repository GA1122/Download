static int find_shared_ctxt(struct file *fp,
			    const struct hfi1_user_info *uinfo)
{
	int devmax, ndev, i;
	int ret = 0;
	struct hfi1_filedata *fd = fp->private_data;

	devmax = hfi1_count_units(NULL, NULL);

	for (ndev = 0; ndev < devmax; ndev++) {
		struct hfi1_devdata *dd = hfi1_lookup(ndev);

		if (!(dd && (dd->flags & HFI1_PRESENT) && dd->kregbase))
			continue;
		for (i = dd->first_user_ctxt; i < dd->num_rcv_contexts; i++) {
			struct hfi1_ctxtdata *uctxt = dd->rcd[i];

			 
			if (!uctxt || !uctxt->cnt)
				continue;
			 
			if (memcmp(uctxt->uuid, uinfo->uuid,
				   sizeof(uctxt->uuid)) ||
			    uctxt->jkey != generate_jkey(current_uid()) ||
			    uctxt->subctxt_id != uinfo->subctxt_id ||
			    uctxt->subctxt_cnt != uinfo->subctxt_cnt)
				continue;

			 
			if (uctxt->userversion != uinfo->userversion ||
			    uctxt->cnt >= uctxt->subctxt_cnt) {
				ret = -EINVAL;
				goto done;
			}
			fd->uctxt = uctxt;
			fd->subctxt  = uctxt->cnt++;
			uctxt->subpid[fd->subctxt] = current->pid;
			uctxt->active_slaves |= 1 << fd->subctxt;
			ret = 1;
			goto done;
		}
	}

done:
	return ret;
}