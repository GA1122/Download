int jas_image_getfmt(jas_stream_t *in)
{
	jas_image_fmtinfo_t *fmtinfo;
	int found;
	int i;

	 
	found = 0;
	for (i = 0, fmtinfo = jas_image_fmtinfos; i < jas_image_numfmts; ++i,
	  ++fmtinfo) {
		if (fmtinfo->ops.validate) {
			 
			JAS_DBGLOG(20, ("testing for format %s ... ", fmtinfo->name));
			if (!(*fmtinfo->ops.validate)(in)) {
				JAS_DBGLOG(20, ("test succeeded\n"));
				found = 1;
				break;
			}
			JAS_DBGLOG(20, ("test failed\n"));
		}
	}
	return found ? fmtinfo->id : (-1);
}
