static bool ndp_msg_opt_route_check_valid(void *opt_data)
{
	struct __nd_opt_route_info *ri = opt_data;

	 
	if (((ri->nd_opt_ri_prf_reserved >> 3) & 3) == 2)
		return false;
	return true;
}
