static int sctp_sendmsg(struct sock *sk, struct msghdr *msg, size_t msg_len)
{
	struct net *net = sock_net(sk);
	struct sctp_sock *sp;
	struct sctp_endpoint *ep;
	struct sctp_association *new_asoc = NULL, *asoc = NULL;
	struct sctp_transport *transport, *chunk_tp;
	struct sctp_chunk *chunk;
	union sctp_addr to;
	struct sockaddr *msg_name = NULL;
	struct sctp_sndrcvinfo default_sinfo;
	struct sctp_sndrcvinfo *sinfo;
	struct sctp_initmsg *sinit;
	sctp_assoc_t associd = 0;
	sctp_cmsgs_t cmsgs = { NULL };
	sctp_scope_t scope;
	bool fill_sinfo_ttl = false, wait_connect = false;
	struct sctp_datamsg *datamsg;
	int msg_flags = msg->msg_flags;
	__u16 sinfo_flags = 0;
	long timeo;
	int err;

	err = 0;
	sp = sctp_sk(sk);
	ep = sp->ep;

	pr_debug("%s: sk:%p, msg:%p, msg_len:%zu ep:%p\n", __func__, sk,
		 msg, msg_len, ep);

	 
	if (sctp_style(sk, TCP) && sctp_sstate(sk, LISTENING)) {
		err = -EPIPE;
		goto out_nounlock;
	}

	 
	err = sctp_msghdr_parse(msg, &cmsgs);
	if (err) {
		pr_debug("%s: msghdr parse err:%x\n", __func__, err);
		goto out_nounlock;
	}

	 
	if (!sctp_style(sk, UDP_HIGH_BANDWIDTH) && msg->msg_name) {
		int msg_namelen = msg->msg_namelen;

		err = sctp_verify_addr(sk, (union sctp_addr *)msg->msg_name,
				       msg_namelen);
		if (err)
			return err;

		if (msg_namelen > sizeof(to))
			msg_namelen = sizeof(to);
		memcpy(&to, msg->msg_name, msg_namelen);
		msg_name = msg->msg_name;
	}

	sinit = cmsgs.init;
	if (cmsgs.sinfo != NULL) {
		memset(&default_sinfo, 0, sizeof(default_sinfo));
		default_sinfo.sinfo_stream = cmsgs.sinfo->snd_sid;
		default_sinfo.sinfo_flags = cmsgs.sinfo->snd_flags;
		default_sinfo.sinfo_ppid = cmsgs.sinfo->snd_ppid;
		default_sinfo.sinfo_context = cmsgs.sinfo->snd_context;
		default_sinfo.sinfo_assoc_id = cmsgs.sinfo->snd_assoc_id;

		sinfo = &default_sinfo;
		fill_sinfo_ttl = true;
	} else {
		sinfo = cmsgs.srinfo;
	}
	 
	if (sinfo) {
		sinfo_flags = sinfo->sinfo_flags;
		associd = sinfo->sinfo_assoc_id;
	}

	pr_debug("%s: msg_len:%zu, sinfo_flags:0x%x\n", __func__,
		 msg_len, sinfo_flags);

	 
	if (sctp_style(sk, TCP) && (sinfo_flags & (SCTP_EOF | SCTP_ABORT))) {
		err = -EINVAL;
		goto out_nounlock;
	}

	 
	if (((sinfo_flags & SCTP_EOF) && (msg_len > 0)) ||
	    (!(sinfo_flags & (SCTP_EOF|SCTP_ABORT)) && (msg_len == 0))) {
		err = -EINVAL;
		goto out_nounlock;
	}

	 
	if ((sinfo_flags & SCTP_ADDR_OVER) && (!msg->msg_name)) {
		err = -EINVAL;
		goto out_nounlock;
	}

	transport = NULL;

	pr_debug("%s: about to look up association\n", __func__);

	lock_sock(sk);

	 
	if (msg_name) {
		 
		asoc = sctp_endpoint_lookup_assoc(ep, &to, &transport);

		 
		if (!asoc &&
		    ((sctp_style(sk, TCP) &&
		      (sctp_sstate(sk, ESTABLISHED) ||
		       sctp_sstate(sk, CLOSING))) ||
		     sctp_endpoint_is_peeled_off(ep, &to))) {
			err = -EADDRNOTAVAIL;
			goto out_unlock;
		}
	} else {
		asoc = sctp_id2assoc(sk, associd);
		if (!asoc) {
			err = -EPIPE;
			goto out_unlock;
		}
	}

	if (asoc) {
		pr_debug("%s: just looked up association:%p\n", __func__, asoc);

		 
		if (sctp_state(asoc, CLOSED) && sctp_style(sk, TCP)) {
			err = -EPIPE;
			goto out_unlock;
		}

		if (sinfo_flags & SCTP_EOF) {
			pr_debug("%s: shutting down association:%p\n",
				 __func__, asoc);

			sctp_primitive_SHUTDOWN(net, asoc, NULL);
			err = 0;
			goto out_unlock;
		}
		if (sinfo_flags & SCTP_ABORT) {

			chunk = sctp_make_abort_user(asoc, msg, msg_len);
			if (!chunk) {
				err = -ENOMEM;
				goto out_unlock;
			}

			pr_debug("%s: aborting association:%p\n",
				 __func__, asoc);

			sctp_primitive_ABORT(net, asoc, chunk);
			err = 0;
			goto out_unlock;
		}
	}

	 
	if (!asoc) {
		pr_debug("%s: there is no association yet\n", __func__);

		if (sinfo_flags & (SCTP_EOF | SCTP_ABORT)) {
			err = -EINVAL;
			goto out_unlock;
		}

		 
		if (sinfo) {
			if (!sinit || !sinit->sinit_num_ostreams) {
				 
				if (sinfo->sinfo_stream >=
				    sp->initmsg.sinit_num_ostreams) {
					err = -EINVAL;
					goto out_unlock;
				}
			} else {
				 
				if (sinfo->sinfo_stream >=
				    sinit->sinit_num_ostreams) {
					err = -EINVAL;
					goto out_unlock;
				}
			}
		}

		 
		if (!ep->base.bind_addr.port) {
			if (sctp_autobind(sk)) {
				err = -EAGAIN;
				goto out_unlock;
			}
		} else {
			 
			if (ep->base.bind_addr.port < inet_prot_sock(net) &&
			    !ns_capable(net->user_ns, CAP_NET_BIND_SERVICE)) {
				err = -EACCES;
				goto out_unlock;
			}
		}

		scope = sctp_scope(&to);
		new_asoc = sctp_association_new(ep, sk, scope, GFP_KERNEL);
		if (!new_asoc) {
			err = -ENOMEM;
			goto out_unlock;
		}
		asoc = new_asoc;
		err = sctp_assoc_set_bind_addr_from_ep(asoc, scope, GFP_KERNEL);
		if (err < 0) {
			err = -ENOMEM;
			goto out_free;
		}

		 
		if (sinit) {
			if (sinit->sinit_num_ostreams) {
				asoc->c.sinit_num_ostreams =
					sinit->sinit_num_ostreams;
			}
			if (sinit->sinit_max_instreams) {
				asoc->c.sinit_max_instreams =
					sinit->sinit_max_instreams;
			}
			if (sinit->sinit_max_attempts) {
				asoc->max_init_attempts
					= sinit->sinit_max_attempts;
			}
			if (sinit->sinit_max_init_timeo) {
				asoc->max_init_timeo =
				 msecs_to_jiffies(sinit->sinit_max_init_timeo);
			}
		}

		 
		transport = sctp_assoc_add_peer(asoc, &to, GFP_KERNEL, SCTP_UNKNOWN);
		if (!transport) {
			err = -ENOMEM;
			goto out_free;
		}
	}

	 
	pr_debug("%s: we have a valid association\n", __func__);

	if (!sinfo) {
		 
		memset(&default_sinfo, 0, sizeof(default_sinfo));
		default_sinfo.sinfo_stream = asoc->default_stream;
		default_sinfo.sinfo_flags = asoc->default_flags;
		default_sinfo.sinfo_ppid = asoc->default_ppid;
		default_sinfo.sinfo_context = asoc->default_context;
		default_sinfo.sinfo_timetolive = asoc->default_timetolive;
		default_sinfo.sinfo_assoc_id = sctp_assoc2id(asoc);

		sinfo = &default_sinfo;
	} else if (fill_sinfo_ttl) {
		 
		sinfo->sinfo_timetolive = asoc->default_timetolive;
	}

	 
	if (msg_len > sk->sk_sndbuf) {
		err = -EMSGSIZE;
		goto out_free;
	}

	if (asoc->pmtu_pending)
		sctp_assoc_pending_pmtu(sk, asoc);

	 
	if (sctp_sk(sk)->disable_fragments && (msg_len > asoc->frag_point)) {
		err = -EMSGSIZE;
		goto out_free;
	}

	 
	if (sinfo->sinfo_stream >= asoc->c.sinit_num_ostreams) {
		err = -EINVAL;
		goto out_free;
	}

	if (sctp_wspace(asoc) < msg_len)
		sctp_prsctp_prune(asoc, sinfo, msg_len - sctp_wspace(asoc));

	timeo = sock_sndtimeo(sk, msg->msg_flags & MSG_DONTWAIT);
	if (!sctp_wspace(asoc)) {
		err = sctp_wait_for_sndbuf(asoc, &timeo, msg_len);
		if (err)
			goto out_free;
	}

	 
	if ((sctp_style(sk, TCP) && msg_name) ||
	    (sinfo_flags & SCTP_ADDR_OVER)) {
		chunk_tp = sctp_assoc_lookup_paddr(asoc, &to);
		if (!chunk_tp) {
			err = -EINVAL;
			goto out_free;
		}
	} else
		chunk_tp = NULL;

	 
	if (sctp_state(asoc, CLOSED)) {
		err = sctp_primitive_ASSOCIATE(net, asoc, NULL);
		if (err < 0)
			goto out_free;

		wait_connect = true;
		pr_debug("%s: we associated primitively\n", __func__);
	}

	 
	datamsg = sctp_datamsg_from_user(asoc, sinfo, &msg->msg_iter);
	if (IS_ERR(datamsg)) {
		err = PTR_ERR(datamsg);
		goto out_free;
	}
	datamsg->force_delay = !!(msg->msg_flags & MSG_MORE);

	 
	list_for_each_entry(chunk, &datamsg->chunks, frag_list) {
		sctp_chunk_hold(chunk);

		 
		sctp_set_owner_w(chunk);

		chunk->transport = chunk_tp;
	}

	 
	err = sctp_primitive_SEND(net, asoc, datamsg);
	 
	if (err) {
		sctp_datamsg_free(datamsg);
		goto out_free;
	}

	pr_debug("%s: we sent primitively\n", __func__);

	sctp_datamsg_put(datamsg);
	err = msg_len;

	if (unlikely(wait_connect)) {
		timeo = sock_sndtimeo(sk, msg_flags & MSG_DONTWAIT);
		sctp_wait_for_connect(asoc, &timeo);
	}

	 
	goto out_unlock;

out_free:
	if (new_asoc)
		sctp_association_free(asoc);
out_unlock:
	release_sock(sk);

out_nounlock:
	return sctp_error(sk, msg_flags, err);

#if 0
do_sock_err:
	if (msg_len)
		err = msg_len;
	else
		err = sock_error(sk);
	goto out;

do_interrupted:
	if (msg_len)
		err = msg_len;
	goto out;
#endif  
}
