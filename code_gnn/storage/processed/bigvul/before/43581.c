static int sctp_setsockopt_rtoinfo(struct sock *sk, char __user *optval, unsigned int optlen)
{
	struct sctp_rtoinfo rtoinfo;
	struct sctp_association *asoc;
	unsigned long rto_min, rto_max;
	struct sctp_sock *sp = sctp_sk(sk);

	if (optlen != sizeof (struct sctp_rtoinfo))
		return -EINVAL;

	if (copy_from_user(&rtoinfo, optval, optlen))
		return -EFAULT;

	asoc = sctp_id2assoc(sk, rtoinfo.srto_assoc_id);

	 
	if (!asoc && rtoinfo.srto_assoc_id && sctp_style(sk, UDP))
		return -EINVAL;

	rto_max = rtoinfo.srto_max;
	rto_min = rtoinfo.srto_min;

	if (rto_max)
		rto_max = asoc ? msecs_to_jiffies(rto_max) : rto_max;
	else
		rto_max = asoc ? asoc->rto_max : sp->rtoinfo.srto_max;

	if (rto_min)
		rto_min = asoc ? msecs_to_jiffies(rto_min) : rto_min;
	else
		rto_min = asoc ? asoc->rto_min : sp->rtoinfo.srto_min;

	if (rto_min > rto_max)
		return -EINVAL;

	if (asoc) {
		if (rtoinfo.srto_initial != 0)
			asoc->rto_initial =
				msecs_to_jiffies(rtoinfo.srto_initial);
		asoc->rto_max = rto_max;
		asoc->rto_min = rto_min;
	} else {
		 
		if (rtoinfo.srto_initial != 0)
			sp->rtoinfo.srto_initial = rtoinfo.srto_initial;
		sp->rtoinfo.srto_max = rto_max;
		sp->rtoinfo.srto_min = rto_min;
	}

	return 0;
}