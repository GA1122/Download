static int srpt_create_ch_ib(struct srpt_rdma_ch *ch)
{
	struct ib_qp_init_attr *qp_init;
	struct srpt_port *sport = ch->sport;
	struct srpt_device *sdev = sport->sdev;
	u32 srp_sq_size = sport->port_attrib.srp_sq_size;
	int ret;

	WARN_ON(ch->rq_size < 1);

	ret = -ENOMEM;
	qp_init = kzalloc(sizeof *qp_init, GFP_KERNEL);
	if (!qp_init)
		goto out;

retry:
	ch->cq = ib_alloc_cq(sdev->device, ch, ch->rq_size + srp_sq_size,
			0  , IB_POLL_WORKQUEUE);
	if (IS_ERR(ch->cq)) {
		ret = PTR_ERR(ch->cq);
		pr_err("failed to create CQ cqe= %d ret= %d\n",
		       ch->rq_size + srp_sq_size, ret);
		goto out;
	}

	qp_init->qp_context = (void *)ch;
	qp_init->event_handler
		= (void(*)(struct ib_event *, void*))srpt_qp_event;
	qp_init->send_cq = ch->cq;
	qp_init->recv_cq = ch->cq;
	qp_init->srq = sdev->srq;
	qp_init->sq_sig_type = IB_SIGNAL_REQ_WR;
	qp_init->qp_type = IB_QPT_RC;
	qp_init->cap.max_send_wr = srp_sq_size;
	qp_init->cap.max_send_sge = SRPT_DEF_SG_PER_WQE;

	ch->qp = ib_create_qp(sdev->pd, qp_init);
	if (IS_ERR(ch->qp)) {
		ret = PTR_ERR(ch->qp);
		if (ret == -ENOMEM) {
			srp_sq_size /= 2;
			if (srp_sq_size >= MIN_SRPT_SQ_SIZE) {
				ib_destroy_cq(ch->cq);
				goto retry;
			}
		}
		pr_err("failed to create_qp ret= %d\n", ret);
		goto err_destroy_cq;
	}

	atomic_set(&ch->sq_wr_avail, qp_init->cap.max_send_wr);

	pr_debug("%s: max_cqe= %d max_sge= %d sq_size = %d cm_id= %p\n",
		 __func__, ch->cq->cqe, qp_init->cap.max_send_sge,
		 qp_init->cap.max_send_wr, ch->cm_id);

	ret = srpt_init_ch_qp(ch, ch->qp);
	if (ret)
		goto err_destroy_qp;

out:
	kfree(qp_init);
	return ret;

err_destroy_qp:
	ib_destroy_qp(ch->qp);
err_destroy_cq:
	ib_free_cq(ch->cq);
	goto out;
}