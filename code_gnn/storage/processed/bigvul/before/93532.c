static struct mr6_table *ip6mr_get_table(struct net *net, u32 id)
{
	struct mr6_table *mrt;

	ip6mr_for_each_table(mrt, net) {
		if (mrt->id == id)
			return mrt;
	}
	return NULL;
}
