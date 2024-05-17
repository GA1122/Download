int rds_ib_send_grab_credits(struct rds_ib_connection *ic,
			     u32 wanted, u32 *adv_credits, int need_posted, int max_posted)
{
	unsigned int avail, posted, got = 0, advertise;
	long oldval, newval;

	*adv_credits = 0;
	if (!ic->i_flowctl)
		return wanted;

try_again:
	advertise = 0;
	oldval = newval = atomic_read(&ic->i_credits);
	posted = IB_GET_POST_CREDITS(oldval);
	avail = IB_GET_SEND_CREDITS(oldval);

	rdsdebug("rds_ib_send_grab_credits(%u): credits=%u posted=%u\n",
			wanted, avail, posted);

	 
	if (avail && !posted)
		avail--;

	if (avail < wanted) {
		struct rds_connection *conn = ic->i_cm_id->context;

		 
		set_bit(RDS_LL_SEND_FULL, &conn->c_flags);
		got = avail;
	} else {
		 
		got = wanted;
	}
	newval -= IB_SET_SEND_CREDITS(got);

	 
	if (posted && (got || need_posted)) {
		advertise = min_t(unsigned int, posted, max_posted);
		newval -= IB_SET_POST_CREDITS(advertise);
	}

	 
	if (atomic_cmpxchg(&ic->i_credits, oldval, newval) != oldval)
		goto try_again;

	*adv_credits = advertise;
	return got;
}