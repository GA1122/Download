static int sample_conv_q_prefered(const struct arg *args, struct sample *smp, void *private)
{
	const char *al = smp->data.u.str.str;
	const char *end = al + smp->data.u.str.len;
	const char *token;
	int toklen;
	int qvalue;
	const char *str;
	const char *w;
	int best_q = 0;

	 
	smp->flags |= SMP_F_CONST;
	smp->data.u.str.size = 0;
	smp->data.u.str.str = "";
	smp->data.u.str.len = 0;

	 
	while (1) {

		 
		while (al < end && isspace((unsigned char)*al))
			al++;
		if (al >= end)
			break;

		 
		token = al;

		 
		while (al < end && *al != ';' && *al != ',' && !isspace((unsigned char)*al))
			al++;
		if (al == token)
			goto expect_comma;

		 
		toklen = al - token;
		qvalue = 1000;

		 
		str = args[0].data.str.str;
		w = str;
		while (1) {
			if (*str == ';' || *str == '\0') {
				if (language_range_match(token, toklen, w, str-w))
					goto look_for_q;
				if (*str == '\0')
					goto expect_comma;
				w = str + 1;
			}
			str++;
		}
		goto expect_comma;

look_for_q:

		 
		while (al < end && isspace((unsigned char)*al))
			al++;
		if (al >= end)
			goto process_value;

		 
		if (*al == ',')
			goto process_value;

		 
		if (*al != ';')
			goto expect_comma;

		 
		al++;

		 
		while (al < end && isspace((unsigned char)*al))
			al++;
		if (al >= end)
			goto process_value;

		 
		if (*al != 'q')
			goto process_value;
		al++;

		 
		while (al < end && isspace((unsigned char)*al))
			al++;
		if (al >= end)
			goto process_value;

		 
		if (*al != '=')
			goto process_value;
		al++;

		 
		while (al < end && isspace((unsigned char)*al))
			al++;
		if (al >= end)
			goto process_value;

		 
		qvalue = parse_qvalue(al, &al);

process_value:

		 
		if (qvalue > best_q) {
			smp->data.u.str.str = (char *)w;
			smp->data.u.str.len = str - w;
			if (qvalue >= 1000)
				break;
			best_q = qvalue;
		}

expect_comma:

		 
		while (al < end && *al != ',')
			al++;
		if (al >= end)
			break;

		 
		al++;
	}

	 
	if (smp->data.u.str.len == 0 && args[1].type == ARGT_STR) {
		smp->data.u.str.str = args[1].data.str.str;
		smp->data.u.str.len = args[1].data.str.len;
	}

	 
	return smp->data.u.str.len != 0;
}
