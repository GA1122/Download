path_isopen(PG_FUNCTION_ARGS)
{
	PATH	   *path = PG_GETARG_PATH_P(0);

	PG_RETURN_BOOL(!path->closed);
}