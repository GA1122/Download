json_each(PG_FUNCTION_ARGS)
{
	return each_worker(fcinfo, false);
}
