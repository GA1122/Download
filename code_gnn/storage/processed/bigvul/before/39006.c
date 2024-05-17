poly_overlap(PG_FUNCTION_ARGS)
{
	POLYGON    *polya = PG_GETARG_POLYGON_P(0);
	POLYGON    *polyb = PG_GETARG_POLYGON_P(1);
	bool		result;

	 
	result = (polya->npts > 0 && polyb->npts > 0 &&
			  box_ov(&polya->boundbox, &polyb->boundbox)) ? true : false;

	 
	if (result)
	{
		int			ia,
					ib;
		LSEG		sa,
					sb;

		 
		sa.p[0] = polya->p[polya->npts - 1];
		result = false;

		for (ia = 0; ia < polya->npts && result == false; ia++)
		{
			 
			sa.p[1] = polya->p[ia];

			 
			sb.p[0] = polyb->p[polyb->npts - 1];

			for (ib = 0; ib < polyb->npts && result == false; ib++)
			{
				sb.p[1] = polyb->p[ib];
				result = lseg_intersect_internal(&sa, &sb);
				sb.p[0] = sb.p[1];
			}

			 
			sa.p[0] = sa.p[1];
		}

		if (result == false)
		{
			result = (point_inside(polya->p, polyb->npts, polyb->p)
					  ||
					  point_inside(polyb->p, polya->npts, polya->p));
		}
	}

	 
	PG_FREE_IF_COPY(polya, 0);
	PG_FREE_IF_COPY(polyb, 1);

	PG_RETURN_BOOL(result);
}
