check_acl(struct mg_context *phys_ctx, uint32_t remote_ip)
{
	int allowed, flag;
	uint32_t net, mask;
	struct vec vec;

	if (phys_ctx) {
		const char *list = phys_ctx->dd.config[ACCESS_CONTROL_LIST];

		 
		allowed = (list == NULL) ? '+' : '-';

		while ((list = next_option(list, &vec, NULL)) != NULL) {
			flag = vec.ptr[0];
			if ((flag != '+' && flag != '-')
			    || (parse_net(&vec.ptr[1], &net, &mask) == 0)) {
				mg_cry_internal(fc(phys_ctx),
				                "%s: subnet must be [+|-]x.x.x.x[/x]",
				                __func__);
				return -1;
			}

			if (net == (remote_ip & mask)) {
				allowed = flag;
			}
		}

		return allowed == '+';
	}
	return -1;
}