poly_contain(PG_FUNCTION_ARGS)
{
	POLYGON    *polya = PG_GETARG_POLYGON_P(0);
	POLYGON    *polyb = PG_GETARG_POLYGON_P(1);
	bool		result;

	 
	if (polya->npts > 0 && polyb->npts > 0 &&
		DatumGetBool(DirectFunctionCall2(box_contain,
										 BoxPGetDatum(&polya->boundbox),
										 BoxPGetDatum(&polyb->boundbox))))
	{
		int			i;
		LSEG		s;

		s.p[0] = polyb->p[polyb->npts - 1];
		result = true;

		for (i = 0; i < polyb->npts && result; i++)
		{
			s.p[1] = polyb->p[i];
			result = lseg_inside_poly(s.p, s.p + 1, polya, 0);
			s.p[0] = s.p[1];
		}
	}
	else
	{
		result = false;
	}

	 
	PG_FREE_IF_COPY(polya, 0);
	PG_FREE_IF_COPY(polyb, 1);

	PG_RETURN_BOOL(result);
}
