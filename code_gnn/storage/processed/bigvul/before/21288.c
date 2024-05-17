struct mempolicy *__mpol_cond_copy(struct mempolicy *tompol,
						struct mempolicy *frompol)
{
	if (!mpol_needs_cond_ref(frompol))
		return frompol;

	*tompol = *frompol;
	tompol->flags &= ~MPOL_F_SHARED;	 
	__mpol_put(frompol);
	return tompol;
}
