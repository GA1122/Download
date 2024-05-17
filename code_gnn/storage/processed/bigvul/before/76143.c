add_seq(char *buf)
{
	char *p = buf + 4;		 
	long one, two, three;
	long start, step, end;
	seq_t *seq_ent;
	char *var;
	char *var_end;

	p += strspn(p, " \t");
	if (*p++ != '(')
		return false;
	p += strspn(p, " \t");

	var = p;

	p += strcspn(p, " \t,)");
	var_end = p;
	p += strspn(p, " \t");
	if (!*p || *p == ')' || p == var) {
		report_config_error(CONFIG_GENERAL_ERROR, "Invalid ~SEQ definition '%s'", buf);
		return false;
	}

	p++;
	do {
		one = strtol(p, &p, 0);
		p += strspn(p, " \t");
		if (*p == ')') {
			end = one;
			step = (end < 1) ? -1 : 1;
			start = (end < 0) ? -1 : 1;

			break;
		}

		if (*p != ',') {
			report_config_error(CONFIG_GENERAL_ERROR, "Invalid ~SEQ definition '%s'", buf);
			return false;
		}

		two = strtol(p + 1, &p, 0);
		p += strspn(p, " \t");
		if (*p == ')') {
			start = one;
			end = two;
			step = start <= end ? 1 : -1;

			break;
		}

		if (*p != ',') {
			report_config_error(CONFIG_GENERAL_ERROR, "Invalid ~SEQ definition '%s'", buf);
			return false;
		}

		three = strtol(p + 1, &p, 0);
		p += strspn(p, " \t");
		if (*p != ')') {
			report_config_error(CONFIG_GENERAL_ERROR, "Invalid ~SEQ definition '%s'", buf);
			return false;
		}

		start = one;
		step = two;
		end = three;

		if (!step ||
		    (start < end && step < 0) ||
		    (start > end && step > 0))
		{
			report_config_error(CONFIG_GENERAL_ERROR, "Invalid ~SEQ values '%s'", buf);
			return false;
		}
	} while (false);

	p += strspn(p + 1, " \t") + 1;

	PMALLOC(seq_ent);
	seq_ent->var = MALLOC(var_end - var + 1);
	strncpy(seq_ent->var, var, var_end - var);
	seq_ent->next = start;
	seq_ent->step = step;
	seq_ent->last = end;
	seq_ent->text = MALLOC(strlen(p) + 1);
	strcpy(seq_ent->text, p);

	if (!seq_list)
		seq_list = alloc_list(free_seq, NULL);
	list_add(seq_list, seq_ent);

	return true;
}