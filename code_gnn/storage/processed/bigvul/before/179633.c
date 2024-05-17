 static void handle_rx(struct vhost_net *net)
 {
 	struct vhost_net_virtqueue *nvq = &net->vqs[VHOST_NET_VQ_RX];
 	struct vhost_virtqueue *vq = &nvq->vq;
 	unsigned uninitialized_var(in), log;
 	struct vhost_log *vq_log;
 	struct msghdr msg = {
 		.msg_name = NULL,
 		.msg_namelen = 0,
 		.msg_control = NULL,  
 		.msg_controllen = 0,
 		.msg_iov = vq->iov,
 		.msg_flags = MSG_DONTWAIT,
 	};
 	struct virtio_net_hdr_mrg_rxbuf hdr = {
 		.hdr.flags = 0,
 		.hdr.gso_type = VIRTIO_NET_HDR_GSO_NONE
 	};
 	size_t total_len = 0;
 	int err, mergeable;
 	s16 headcount;
 	size_t vhost_hlen, sock_hlen;
 	size_t vhost_len, sock_len;
 	struct socket *sock;
 
 	mutex_lock(&vq->mutex);
 	sock = vq->private_data;
 	if (!sock)
 		goto out;
 	vhost_disable_notify(&net->dev, vq);
 
 	vhost_hlen = nvq->vhost_hlen;
 	sock_hlen = nvq->sock_hlen;
 
 	vq_log = unlikely(vhost_has_feature(&net->dev, VHOST_F_LOG_ALL)) ?
 		vq->log : NULL;
 	mergeable = vhost_has_feature(&net->dev, VIRTIO_NET_F_MRG_RXBUF);
 
 	while ((sock_len = peek_head_len(sock->sk))) {
 		sock_len += sock_hlen;
 		vhost_len = sock_len + vhost_hlen;
 		headcount = get_rx_bufs(vq, vq->heads, vhost_len,
 					&in, vq_log, &log,
 					likely(mergeable) ? UIO_MAXIOV : 1);
  		 
  		if (unlikely(headcount < 0))
  			break;
// 		 
// 		if (unlikely(headcount > UIO_MAXIOV)) {
// 			msg.msg_iovlen = 1;
// 			err = sock->ops->recvmsg(NULL, sock, &msg,
// 						 1, MSG_DONTWAIT | MSG_TRUNC);
// 			pr_debug("Discarded rx packet: len %zd\n", sock_len);
// 			continue;
// 		}
  		 
  		if (!headcount) {
  			if (unlikely(vhost_enable_notify(&net->dev, vq))) {
 				 
 				vhost_disable_notify(&net->dev, vq);
 				continue;
 			}
 			 
 			break;
 		}
 		 
 		if (unlikely((vhost_hlen)))
 			 
 			move_iovec_hdr(vq->iov, nvq->hdr, vhost_hlen, in);
 		else
 			 
 			copy_iovec_hdr(vq->iov, nvq->hdr, sock_hlen, in);
 		msg.msg_iovlen = in;
 		err = sock->ops->recvmsg(NULL, sock, &msg,
 					 sock_len, MSG_DONTWAIT | MSG_TRUNC);
 		 
 		if (unlikely(err != sock_len)) {
 			pr_debug("Discarded rx packet: "
 				 " len %d, expected %zd\n", err, sock_len);
 			vhost_discard_vq_desc(vq, headcount);
 			continue;
 		}
 		if (unlikely(vhost_hlen) &&
 		    memcpy_toiovecend(nvq->hdr, (unsigned char *)&hdr, 0,
 				      vhost_hlen)) {
 			vq_err(vq, "Unable to write vnet_hdr at addr %p\n",
 			       vq->iov->iov_base);
 			break;
 		}
 		 
 		if (likely(mergeable) &&
 		    memcpy_toiovecend(nvq->hdr, (unsigned char *)&headcount,
 				      offsetof(typeof(hdr), num_buffers),
 				      sizeof hdr.num_buffers)) {
 			vq_err(vq, "Failed num_buffers write");
 			vhost_discard_vq_desc(vq, headcount);
 			break;
 		}
 		vhost_add_used_and_signal_n(&net->dev, vq, vq->heads,
 					    headcount);
 		if (unlikely(vq_log))
 			vhost_log_write(vq, vq_log, log, vhost_len);
 		total_len += vhost_len;
 		if (unlikely(total_len >= VHOST_NET_WEIGHT)) {
 			vhost_poll_queue(&vq->poll);
 			break;
 		}
 	}
 out:
 	mutex_unlock(&vq->mutex);
 }