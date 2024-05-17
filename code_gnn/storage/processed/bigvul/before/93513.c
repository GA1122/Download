static struct mfc6_cache *ip6mr_cache_alloc(void)
{
	struct mfc6_cache *c = kmem_cache_zalloc(mrt_cachep, GFP_KERNEL);
	if (!c)
		return NULL;
	c->mfc_un.res.last_assert = jiffies - MFC_ASSERT_THRESH - 1;
	c->mfc_un.res.minvif = MAXMIFS;
	return c;
}