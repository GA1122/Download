int iwch_accept_cr(struct iw_cm_id *cm_id, struct iw_cm_conn_param *conn_param)
{
	int err;
	struct iwch_qp_attributes attrs;
	enum iwch_qp_attr_mask mask;
	struct iwch_ep *ep = to_ep(cm_id);
	struct iwch_dev *h = to_iwch_dev(cm_id->device);
	struct iwch_qp *qp = get_qhp(h, conn_param->qpn);

	PDBG("%s ep %p tid %u\n", __func__, ep, ep->hwtid);
	if (state_read(&ep->com) == DEAD) {
		err = -ECONNRESET;
		goto err;
	}

	BUG_ON(state_read(&ep->com) != MPA_REQ_RCVD);
	BUG_ON(!qp);

	if ((conn_param->ord > qp->rhp->attr.max_rdma_read_qp_depth) ||
	    (conn_param->ird > qp->rhp->attr.max_rdma_reads_per_qp)) {
		abort_connection(ep, NULL, GFP_KERNEL);
		err = -EINVAL;
		goto err;
	}

	cm_id->add_ref(cm_id);
	ep->com.cm_id = cm_id;
	ep->com.qp = qp;

	ep->ird = conn_param->ird;
	ep->ord = conn_param->ord;

	if (peer2peer && ep->ird == 0)
		ep->ird = 1;

	PDBG("%s %d ird %d ord %d\n", __func__, __LINE__, ep->ird, ep->ord);

	 
	attrs.mpa_attr = ep->mpa_attr;
	attrs.max_ird = ep->ird;
	attrs.max_ord = ep->ord;
	attrs.llp_stream_handle = ep;
	attrs.next_state = IWCH_QP_STATE_RTS;

	 
	mask = IWCH_QP_ATTR_NEXT_STATE |
			     IWCH_QP_ATTR_LLP_STREAM_HANDLE |
			     IWCH_QP_ATTR_MPA_ATTR |
			     IWCH_QP_ATTR_MAX_IRD |
			     IWCH_QP_ATTR_MAX_ORD;

	err = iwch_modify_qp(ep->com.qp->rhp,
			     ep->com.qp, mask, &attrs, 1);
	if (err)
		goto err1;

	 
	if (iwch_rqes_posted(qp)) {
		wait_event(ep->com.waitq, ep->com.rpl_done);
		err = ep->com.rpl_err;
		if (err)
			goto err1;
	}

	err = send_mpa_reply(ep, conn_param->private_data,
			     conn_param->private_data_len);
	if (err)
		goto err1;


	state_set(&ep->com, FPDU_MODE);
	established_upcall(ep);
	put_ep(&ep->com);
	return 0;
err1:
	ep->com.cm_id = NULL;
	ep->com.qp = NULL;
	cm_id->rem_ref(cm_id);
err:
	put_ep(&ep->com);
	return err;
}