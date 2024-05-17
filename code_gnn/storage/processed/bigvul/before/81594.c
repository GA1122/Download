static int parse_pred(const char *str, void *data,
		      int pos, struct filter_parse_error *pe,
		      struct filter_pred **pred_ptr)
{
	struct trace_event_call *call = data;
	struct ftrace_event_field *field;
	struct filter_pred *pred = NULL;
	char num_buf[24];	 
	char *field_name;
	char q;
	u64 val;
	int len;
	int ret;
	int op;
	int s;
	int i = 0;

	 
	while (isspace(str[i]))
		i++;
	s = i;

	while (isalnum(str[i]) || str[i] == '_')
		i++;

	len = i - s;

	if (!len)
		return -1;

	field_name = kmemdup_nul(str + s, len, GFP_KERNEL);
	if (!field_name)
		return -ENOMEM;

	 

	field = trace_find_event_field(call, field_name);
	kfree(field_name);
	if (!field) {
		parse_error(pe, FILT_ERR_FIELD_NOT_FOUND, pos + i);
		return -EINVAL;
	}

	while (isspace(str[i]))
		i++;

	 
	for (op = 0; ops[op]; op++) {
		 
		if (strncmp(str + i, ops[op], strlen(ops[op])) == 0)
			break;
	}

	if (!ops[op]) {
		parse_error(pe, FILT_ERR_INVALID_OP, pos + i);
		goto err_free;
	}

	i += strlen(ops[op]);

	while (isspace(str[i]))
		i++;

	s = i;

	pred = kzalloc(sizeof(*pred), GFP_KERNEL);
	if (!pred)
		return -ENOMEM;

	pred->field = field;
	pred->offset = field->offset;
	pred->op = op;

	if (ftrace_event_is_function(call)) {
		 
		if (strcmp(field->name, "ip") != 0) {
			 parse_error(pe, FILT_ERR_IP_FIELD_ONLY, pos + i);
			 goto err_free;
		 }
		 pred->fn = filter_pred_none;

		  
		 if (str[i] == '\'' || str[i] == '"')
			 q = str[i];
		 else
			 q = 0;

		 for (i++; str[i]; i++) {
			 if (q && str[i] == q)
				 break;
			 if (!q && (str[i] == ')' || str[i] == '&' ||
				    str[i] == '|'))
				 break;
		 }
		  
		 if (q)
			 s++;
		len = i - s;
		if (len >= MAX_FILTER_STR_VAL) {
			parse_error(pe, FILT_ERR_OPERAND_TOO_LONG, pos + i);
			goto err_free;
		}

		pred->regex.len = len;
		strncpy(pred->regex.pattern, str + s, len);
		pred->regex.pattern[len] = 0;

	 
	} else if (str[i] == '\'' || str[i] == '"') {
		char q = str[i];

		 
		switch (op) {
		case OP_NE:
			pred->not = 1;
			 
		case OP_GLOB:
		case OP_EQ:
			break;
		default:
			parse_error(pe, FILT_ERR_ILLEGAL_FIELD_OP, pos + i);
			goto err_free;
		}

		 
		if (!is_string_field(field)) {
			parse_error(pe, FILT_ERR_EXPECT_DIGIT, pos + i);
			goto err_free;
		}

		for (i++; str[i]; i++) {
			if (str[i] == q)
				break;
		}
		if (!str[i]) {
			parse_error(pe, FILT_ERR_MISSING_QUOTE, pos + i);
			goto err_free;
		}

		 
		s++;
		len = i - s;
		if (len >= MAX_FILTER_STR_VAL) {
			parse_error(pe, FILT_ERR_OPERAND_TOO_LONG, pos + i);
			goto err_free;
		}

		pred->regex.len = len;
		strncpy(pred->regex.pattern, str + s, len);
		pred->regex.pattern[len] = 0;

		filter_build_regex(pred);

		if (field->filter_type == FILTER_COMM) {
			pred->fn = filter_pred_comm;

		} else if (field->filter_type == FILTER_STATIC_STRING) {
			pred->fn = filter_pred_string;
			pred->regex.field_len = field->size;

		} else if (field->filter_type == FILTER_DYN_STRING)
			pred->fn = filter_pred_strloc;
		else
			pred->fn = filter_pred_pchar;
		 
		i++;

	} else if (isdigit(str[i])) {

		 
		if (is_string_field(field)) {
			parse_error(pe, FILT_ERR_EXPECT_STRING, pos + i);
			goto err_free;
		}

		if (op == OP_GLOB) {
			parse_error(pe, FILT_ERR_ILLEGAL_FIELD_OP, pos + i);
			goto err_free;
		}

		 
		while (isalnum(str[i]))
			i++;

		len = i - s;
		 
		if (len >= sizeof(num_buf)) {
			parse_error(pe, FILT_ERR_OPERAND_TOO_LONG, pos + i);
			goto err_free;
		}

		strncpy(num_buf, str + s, len);
		num_buf[len] = 0;

		 
		if (field->is_signed)
			ret = kstrtoll(num_buf, 0, &val);
		else
			ret = kstrtoull(num_buf, 0, &val);
		if (ret) {
			parse_error(pe, FILT_ERR_ILLEGAL_INTVAL, pos + s);
			goto err_free;
		}

		pred->val = val;

		if (field->filter_type == FILTER_CPU)
			pred->fn = filter_pred_cpu;
		else {
			pred->fn = select_comparison_fn(pred->op, field->size,
							field->is_signed);
			if (pred->op == OP_NE)
				pred->not = 1;
		}

	} else {
		parse_error(pe, FILT_ERR_INVALID_VALUE, pos + i);
		goto err_free;
	}

	*pred_ptr = pred;
	return i;

err_free:
	kfree(pred);
	return -EINVAL;
}
