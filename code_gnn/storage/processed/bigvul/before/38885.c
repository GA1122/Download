dist_ppath(PG_FUNCTION_ARGS)
{
	Point	   *pt = PG_GETARG_POINT_P(0);
	PATH	   *path = PG_GETARG_PATH_P(1);
	float8		result = 0.0;	 
	bool		have_min = false;
	float8		tmp;
	int			i;
	LSEG		lseg;

	switch (path->npts)
	{
		case 0:
			 
			PG_RETURN_NULL();
		case 1:
			 
			result = point_dt(pt, &path->p[0]);
			break;
		default:
			 
			Assert(path->npts > 1);

			 
			for (i = 0; i < path->npts; i++)
			{
				int			iprev;

				if (i > 0)
					iprev = i - 1;
				else
				{
					if (!path->closed)
						continue;
					iprev = path->npts - 1;		 
				}

				statlseg_construct(&lseg, &path->p[iprev], &path->p[i]);
				tmp = dist_ps_internal(pt, &lseg);
				if (!have_min || tmp < result)
				{
					result = tmp;
					have_min = true;
				}
			}
			break;
	}
	PG_RETURN_FLOAT8(result);
}
