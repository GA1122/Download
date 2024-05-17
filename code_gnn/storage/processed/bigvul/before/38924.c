lseg_interpt_internal(LSEG *l1, LSEG *l2)
{
	Point	   *result;
	LINE		tmp1,
				tmp2;

	 
	line_construct_pts(&tmp1, &l1->p[0], &l1->p[1]);
	line_construct_pts(&tmp2, &l2->p[0], &l2->p[1]);
	result = line_interpt_internal(&tmp1, &tmp2);
	if (!PointerIsValid(result))
		return NULL;

	 
	if (!on_ps_internal(result, l1) ||
		!on_ps_internal(result, l2))
	{
		pfree(result);
		return NULL;
	}

	 
	if ((FPeq(l1->p[0].x, l2->p[0].x) && FPeq(l1->p[0].y, l2->p[0].y)) ||
		(FPeq(l1->p[0].x, l2->p[1].x) && FPeq(l1->p[0].y, l2->p[1].y)))
	{
		result->x = l1->p[0].x;
		result->y = l1->p[0].y;
	}
	else if ((FPeq(l1->p[1].x, l2->p[0].x) && FPeq(l1->p[1].y, l2->p[0].y)) ||
			 (FPeq(l1->p[1].x, l2->p[1].x) && FPeq(l1->p[1].y, l2->p[1].y)))
	{
		result->x = l1->p[1].x;
		result->y = l1->p[1].y;
	}

	return result;
}
