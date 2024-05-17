static int sctp_setsockopt_delayed_ack(struct sock *sk,
				       char __user *optval, unsigned int optlen)
{
	struct sctp_sack_info    params;
	struct sctp_transport   *trans = NULL;
	struct sctp_association *asoc = NULL;
	struct sctp_sock        *sp = sctp_sk(sk);

	if (optlen == sizeof(struct sctp_sack_info)) {
		if (copy_from_user(&params, optval, optlen))
			return -EFAULT;

		if (params.sack_delay == 0 && params.sack_freq == 0)
			return 0;
	} else if (optlen == sizeof(struct sctp_assoc_value)) {
		pr_warn("Use of struct sctp_assoc_value in delayed_ack socket option deprecated\n");
		pr_warn("Use struct sctp_sack_info instead\n");
		if (copy_from_user(&params, optval, optlen))
			return -EFAULT;

		if (params.sack_delay == 0)
			params.sack_freq = 1;
		else
			params.sack_freq = 0;
	} else
		return - EINVAL;

	 
	if (params.sack_delay > 500)
		return -EINVAL;

	 
	asoc = sctp_id2assoc(sk, params.sack_assoc_id);
	if (!asoc && params.sack_assoc_id && sctp_style(sk, UDP))
		return -EINVAL;

	if (params.sack_delay) {
		if (asoc) {
			asoc->sackdelay =
				msecs_to_jiffies(params.sack_delay);
			asoc->param_flags =
				(asoc->param_flags & ~SPP_SACKDELAY) |
				SPP_SACKDELAY_ENABLE;
		} else {
			sp->sackdelay = params.sack_delay;
			sp->param_flags =
				(sp->param_flags & ~SPP_SACKDELAY) |
				SPP_SACKDELAY_ENABLE;
		}
	}

	if (params.sack_freq == 1) {
		if (asoc) {
			asoc->param_flags =
				(asoc->param_flags & ~SPP_SACKDELAY) |
				SPP_SACKDELAY_DISABLE;
		} else {
			sp->param_flags =
				(sp->param_flags & ~SPP_SACKDELAY) |
				SPP_SACKDELAY_DISABLE;
		}
	} else if (params.sack_freq > 1) {
		if (asoc) {
			asoc->sackfreq = params.sack_freq;
			asoc->param_flags =
				(asoc->param_flags & ~SPP_SACKDELAY) |
				SPP_SACKDELAY_ENABLE;
		} else {
			sp->sackfreq = params.sack_freq;
			sp->param_flags =
				(sp->param_flags & ~SPP_SACKDELAY) |
				SPP_SACKDELAY_ENABLE;
		}
	}

	 
	if (asoc) {
		list_for_each_entry(trans, &asoc->peer.transport_addr_list,
				transports) {
			if (params.sack_delay) {
				trans->sackdelay =
					msecs_to_jiffies(params.sack_delay);
				trans->param_flags =
					(trans->param_flags & ~SPP_SACKDELAY) |
					SPP_SACKDELAY_ENABLE;
			}
			if (params.sack_freq == 1) {
				trans->param_flags =
					(trans->param_flags & ~SPP_SACKDELAY) |
					SPP_SACKDELAY_DISABLE;
			} else if (params.sack_freq > 1) {
				trans->sackfreq = params.sack_freq;
				trans->param_flags =
					(trans->param_flags & ~SPP_SACKDELAY) |
					SPP_SACKDELAY_ENABLE;
			}
		}
	}

	return 0;
}