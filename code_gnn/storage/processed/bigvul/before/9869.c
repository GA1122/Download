smp_prefetch_http(struct proxy *px, struct session *s, void *l7, unsigned int opt,
                  const struct arg *args, struct sample *smp, int req_vol)
{
	struct http_txn *txn = l7;
	struct http_msg *msg = &txn->req;

	 

	if (unlikely(!s || !txn))
		return 0;

	 
	smp->type = SMP_T_BOOL;

	if ((opt & SMP_OPT_DIR) == SMP_OPT_DIR_REQ) {
		if (unlikely(!s->req))
			return 0;

		 
		if (s->req->buf->p > s->req->buf->data &&
		    s->req->buf->i + s->req->buf->p > s->req->buf->data + s->req->buf->size - global.tune.maxrewrite)
			buffer_slow_realign(s->req->buf);

		if (unlikely(txn->req.msg_state < HTTP_MSG_BODY)) {
			if (msg->msg_state == HTTP_MSG_ERROR)
				return 0;

			 
			if (likely(msg->next < s->req->buf->i))
				http_msg_analyzer(msg, &txn->hdr_idx);

			 
			if (unlikely(msg->msg_state < HTTP_MSG_BODY)) {
				if ((msg->msg_state == HTTP_MSG_ERROR) ||
				    buffer_full(s->req->buf, global.tune.maxrewrite)) {
					return 0;
				}
				 
				smp->flags |= SMP_F_MAY_CHANGE;
				return 0;
			}

			 

			 
			if (unlikely(s->req->buf->i + s->req->buf->p >
				     s->req->buf->data + s->req->buf->size - global.tune.maxrewrite)) {
				msg->msg_state = HTTP_MSG_ERROR;
				smp->data.uint = 1;
				return 1;
			}

			txn->meth = find_http_meth(msg->chn->buf->p, msg->sl.rq.m_l);
			if (txn->meth == HTTP_METH_GET || txn->meth == HTTP_METH_HEAD)
				s->flags |= SN_REDIRECTABLE;

			if (unlikely(msg->sl.rq.v_l == 0) && !http_upgrade_v09_to_v10(txn))
				return 0;
		}

		if (req_vol && txn->rsp.msg_state != HTTP_MSG_RPBEFORE) {
			return 0;   
		}

		 
	}
	else {
		 
		if (txn->rsp.msg_state < HTTP_MSG_BODY) {
			smp->flags |= SMP_F_MAY_CHANGE;
			return 0;
		}
	}

	 
	smp->data.uint = 1;
	return 1;
}