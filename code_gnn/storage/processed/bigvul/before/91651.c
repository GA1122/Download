void bgp_attr_unintern(struct attr **pattr)
{
	struct attr *attr = *pattr;
	struct attr *ret;
	struct attr tmp;

	 
	attr->refcnt--;

	tmp = *attr;

	 
	if (attr->refcnt == 0) {
		ret = hash_release(attrhash, attr);
		assert(ret != NULL);
		XFREE(MTYPE_ATTR, attr);
		*pattr = NULL;
	}

	bgp_attr_unintern_sub(&tmp);
}
