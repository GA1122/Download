void xs_init_match(struct xtables_match *match)
{
	if (match->udata_size != 0) {
		 
		free(match->udata);
		match->udata = calloc(1, match->udata_size);
		if (match->udata == NULL)
			xtables_error(RESOURCE_PROBLEM, "malloc");
	}
	if (match->init != NULL)
		match->init(match->m);
}
