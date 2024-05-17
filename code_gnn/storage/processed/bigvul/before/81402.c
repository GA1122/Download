int trace_get_user(struct trace_parser *parser, const char __user *ubuf,
	size_t cnt, loff_t *ppos)
{
	char ch;
	size_t read = 0;
	ssize_t ret;

	if (!*ppos)
		trace_parser_clear(parser);

	ret = get_user(ch, ubuf++);
	if (ret)
		goto out;

	read++;
	cnt--;

	 
	if (!parser->cont) {
		 
		while (cnt && isspace(ch)) {
			ret = get_user(ch, ubuf++);
			if (ret)
				goto out;
			read++;
			cnt--;
		}

		parser->idx = 0;

		 
		if (isspace(ch) || !ch) {
			*ppos += read;
			ret = read;
			goto out;
		}
	}

	 
	while (cnt && !isspace(ch) && ch) {
		if (parser->idx < parser->size - 1)
			parser->buffer[parser->idx++] = ch;
		else {
			ret = -EINVAL;
			goto out;
		}
		ret = get_user(ch, ubuf++);
		if (ret)
			goto out;
		read++;
		cnt--;
	}

	 
	if (isspace(ch) || !ch) {
		parser->buffer[parser->idx] = 0;
		parser->cont = false;
	} else if (parser->idx < parser->size - 1) {
		parser->cont = true;
		parser->buffer[parser->idx++] = ch;
		 
		parser->buffer[parser->idx] = 0;
	} else {
		ret = -EINVAL;
		goto out;
	}

	*ppos += read;
	ret = read;

out:
	return ret;
}
