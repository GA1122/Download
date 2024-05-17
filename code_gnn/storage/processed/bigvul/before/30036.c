static void handle_tx(struct vhost_net *net)
{
	struct vhost_net_virtqueue *nvq = &net->vqs[VHOST_NET_VQ_TX];
	struct vhost_virtqueue *vq = &nvq->vq;
	unsigned out, in, s;
	int head;
	struct msghdr msg = {
		.msg_name = NULL,
		.msg_namelen = 0,
		.msg_control = NULL,
		.msg_controllen = 0,
		.msg_iov = vq->iov,
		.msg_flags = MSG_DONTWAIT,
	};
	size_t len, total_len = 0;
	int err;
	size_t hdr_size;
	struct socket *sock;
	struct vhost_net_ubuf_ref *uninitialized_var(ubufs);
	bool zcopy, zcopy_used;

	 
	sock = rcu_dereference_check(vq->private_data, 1);
	if (!sock)
		return;

	mutex_lock(&vq->mutex);
	vhost_disable_notify(&net->dev, vq);

	hdr_size = nvq->vhost_hlen;
	zcopy = nvq->ubufs;

	for (;;) {
		 
		if (zcopy)
			vhost_zerocopy_signal_used(net, vq);

		head = vhost_get_vq_desc(&net->dev, vq, vq->iov,
					 ARRAY_SIZE(vq->iov),
					 &out, &in,
					 NULL, NULL);
		 
		if (unlikely(head < 0))
			break;
		 
		if (head == vq->num) {
			int num_pends;

			 
			num_pends = likely(nvq->upend_idx >= nvq->done_idx) ?
				    (nvq->upend_idx - nvq->done_idx) :
				    (nvq->upend_idx + UIO_MAXIOV -
				     nvq->done_idx);
			if (unlikely(num_pends > VHOST_MAX_PEND))
				break;
			if (unlikely(vhost_enable_notify(&net->dev, vq))) {
				vhost_disable_notify(&net->dev, vq);
				continue;
			}
			break;
		}
		if (in) {
			vq_err(vq, "Unexpected descriptor format for TX: "
			       "out %d, int %d\n", out, in);
			break;
		}
		 
		s = move_iovec_hdr(vq->iov, nvq->hdr, hdr_size, out);
		msg.msg_iovlen = out;
		len = iov_length(vq->iov, out);
		 
		if (!len) {
			vq_err(vq, "Unexpected header len for TX: "
			       "%zd expected %zd\n",
			       iov_length(nvq->hdr, s), hdr_size);
			break;
		}
		zcopy_used = zcopy && (len >= VHOST_GOODCOPY_LEN ||
				       nvq->upend_idx != nvq->done_idx);

		 
		if (zcopy_used) {
			vq->heads[nvq->upend_idx].id = head;
			if (!vhost_net_tx_select_zcopy(net) ||
			    len < VHOST_GOODCOPY_LEN) {
				 
				vq->heads[nvq->upend_idx].len =
							VHOST_DMA_DONE_LEN;
				msg.msg_control = NULL;
				msg.msg_controllen = 0;
				ubufs = NULL;
			} else {
				struct ubuf_info *ubuf;
				ubuf = nvq->ubuf_info + nvq->upend_idx;

				vq->heads[nvq->upend_idx].len =
					VHOST_DMA_IN_PROGRESS;
				ubuf->callback = vhost_zerocopy_callback;
				ubuf->ctx = nvq->ubufs;
				ubuf->desc = nvq->upend_idx;
				msg.msg_control = ubuf;
				msg.msg_controllen = sizeof(ubuf);
				ubufs = nvq->ubufs;
				kref_get(&ubufs->kref);
			}
			nvq->upend_idx = (nvq->upend_idx + 1) % UIO_MAXIOV;
		} else
			msg.msg_control = NULL;
		 
		err = sock->ops->sendmsg(NULL, sock, &msg, len);
		if (unlikely(err < 0)) {
			if (zcopy_used) {
				if (ubufs)
					vhost_net_ubuf_put(ubufs);
				nvq->upend_idx = ((unsigned)nvq->upend_idx - 1)
					% UIO_MAXIOV;
			}
			vhost_discard_vq_desc(vq, 1);
			break;
		}
		if (err != len)
			pr_debug("Truncated TX packet: "
				 " len %d != %zd\n", err, len);
		if (!zcopy_used)
			vhost_add_used_and_signal(&net->dev, vq, head, 0);
		else
			vhost_zerocopy_signal_used(net, vq);
		total_len += len;
		vhost_net_tx_packet(net);
		if (unlikely(total_len >= VHOST_NET_WEIGHT)) {
			vhost_poll_queue(&vq->poll);
			break;
		}
	}

	mutex_unlock(&vq->mutex);
}
