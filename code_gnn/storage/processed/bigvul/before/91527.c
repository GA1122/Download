int gd_getin (void *in)
{
 	return (gdGetC((gdIOCtx *) in));
 }