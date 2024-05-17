static inline gfp_t gfp_exact_node(gfp_t flags)
{
	return flags & ~__GFP_NOFAIL;
}
