get_chainname_rulenum(const struct ipt_entry *s, const struct ipt_entry *e,
		      const char *hookname, const char **chainname,
		      const char **comment, unsigned int *rulenum)
{
	const struct xt_standard_target *t = (void *)ipt_get_target_c(s);

	if (strcmp(t->target.u.kernel.target->name, XT_ERROR_TARGET) == 0) {
		 
		*chainname = t->target.data;
		(*rulenum) = 0;
	} else if (s == e) {
		(*rulenum)++;

		if (s->target_offset == sizeof(struct ipt_entry) &&
		    strcmp(t->target.u.kernel.target->name,
			   XT_STANDARD_TARGET) == 0 &&
		   t->verdict < 0 &&
		   unconditional(&s->ip)) {
			 
			*comment = *chainname == hookname
				? comments[NF_IP_TRACE_COMMENT_POLICY]
				: comments[NF_IP_TRACE_COMMENT_RETURN];
		}
		return 1;
	} else
		(*rulenum)++;

	return 0;
}