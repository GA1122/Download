static struct taskstats *mk_reply(struct sk_buff *skb, int type, u32 pid)
{
	struct nlattr *na, *ret;
	int aggr;

	aggr = (type == TASKSTATS_TYPE_PID)
			? TASKSTATS_TYPE_AGGR_PID
			: TASKSTATS_TYPE_AGGR_TGID;

	 
#ifdef TASKSTATS_NEEDS_PADDING
	if (nla_put(skb, TASKSTATS_TYPE_NULL, 0, NULL) < 0)
		goto err;
#endif
	na = nla_nest_start(skb, aggr);
	if (!na)
		goto err;

	if (nla_put(skb, type, sizeof(pid), &pid) < 0)
		goto err;
	ret = nla_reserve(skb, TASKSTATS_TYPE_STATS, sizeof(struct taskstats));
	if (!ret)
		goto err;
	nla_nest_end(skb, na);

	return nla_data(ret);
err:
	return NULL;
}
