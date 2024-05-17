close_sb(PG_FUNCTION_ARGS)
{
	LSEG	   *lseg = PG_GETARG_LSEG_P(0);
	BOX		   *box = PG_GETARG_BOX_P(1);
	Point		point;
	LSEG		bseg,
				seg;
	double		dist,
				d;

	 
	if (DatumGetBool(DirectFunctionCall2(inter_sb,
										 LsegPGetDatum(lseg),
										 BoxPGetDatum(box))))
	{
		box_cn(&point, box);
		PG_RETURN_DATUM(DirectFunctionCall2(close_ps,
											PointPGetDatum(&point),
											LsegPGetDatum(lseg)));
	}

	 
	point.x = box->low.x;
	point.y = box->high.y;
	statlseg_construct(&bseg, &box->low, &point);
	dist = lseg_dt(lseg, &bseg);

	statlseg_construct(&seg, &box->high, &point);
	if ((d = lseg_dt(lseg, &seg)) < dist)
	{
		dist = d;
		memcpy(&bseg, &seg, sizeof(bseg));
	}

	point.x = box->high.x;
	point.y = box->low.y;
	statlseg_construct(&seg, &box->low, &point);
	if ((d = lseg_dt(lseg, &seg)) < dist)
	{
		dist = d;
		memcpy(&bseg, &seg, sizeof(bseg));
	}

	statlseg_construct(&seg, &box->high, &point);
	if ((d = lseg_dt(lseg, &seg)) < dist)
	{
		dist = d;
		memcpy(&bseg, &seg, sizeof(bseg));
	}

	 
	PG_RETURN_DATUM(DirectFunctionCall2(close_lseg,
										LsegPGetDatum(lseg),
										LsegPGetDatum(&bseg)));
}
