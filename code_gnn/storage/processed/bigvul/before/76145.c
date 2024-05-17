alloc_strvec_quoted_escaped(char *src)
{
	char *token;
	vector_t *strvec;
	char cur_quote = 0;
	char *ofs_op;
	char *op_buf;
	char *ofs, *ofs1;
	char op_char;

	if (!src) {
		if (!buf_extern)
			return NULL;
		src = buf_extern;
	}

	 
	strvec = vector_alloc();
	op_buf = MALLOC(MAXBUF);

	ofs = src;
	while (*ofs) {
		 
		ofs += strspn(ofs, WHITE_SPACE);
		if (!*ofs)
			break;

		ofs_op = op_buf;

		while (*ofs) {
			ofs1 = strpbrk(ofs, cur_quote == '"' ? "\"\\" : cur_quote == '\'' ? "'\\" : WHITE_SPACE_STR "'\"\\");

			if (!ofs1) {
				size_t len;
				if (cur_quote) {
					report_config_error(CONFIG_UNMATCHED_QUOTE, "String '%s': missing terminating %c", src, cur_quote);
					goto err_exit;
				}
				strcpy(ofs_op, ofs);
				len =  strlen(ofs);
				ofs += len;
				ofs_op += len;
				break;
			}

			 
			strncpy(ofs_op, ofs, ofs1 - ofs);
			ofs_op += ofs1 - ofs;
			ofs = ofs1;

			if (*ofs == '\\') {
				 
				ofs++;

				if (!*ofs) {
					log_message(LOG_INFO, "Missing escape char at end: '%s'", src);
					goto err_exit;
				}

				if (*ofs == 'x' && isxdigit(ofs[1])) {
					op_char = 0;
					ofs++;
					while (isxdigit(*ofs)) {
						op_char <<= 4;
						op_char |= isdigit(*ofs) ? *ofs - '0' : (10 + *ofs - (isupper(*ofs)  ? 'A' : 'a'));
						ofs++;
					}
				}
				else if (*ofs == 'c' && ofs[1]) {
					op_char = *++ofs & 0x1f;	 
					ofs++;
				}
				else if (*ofs >= '0' && *ofs <= '7') {
					op_char = *ofs++ - '0';
					if (*ofs >= '0' && *ofs <= '7') {
						op_char <<= 3;
						op_char += *ofs++ - '0';
					}
					if (*ofs >= '0' && *ofs <= '7') {
						op_char <<= 3;
						op_char += *ofs++ - '0';
					}
				}
				else {
					switch (*ofs) {
					case 'a':
						op_char = '\a';
						break;
					case 'b':
						op_char = '\b';
						break;
					case 'E':
						op_char = 0x1b;
						break;
					case 'f':
						op_char = '\f';
						break;
					case 'n':
						op_char = '\n';
						break;
					case 'r':
						op_char = '\r';
						break;
					case 't':
						op_char = '\t';
						break;
					case 'v':
						op_char = '\v';
						break;
					default:  
						op_char = *ofs;
						break;
					}
					ofs++;
				}

				*ofs_op++ = op_char;
				continue;
			}

			if (cur_quote) {
				 
				ofs++;
				cur_quote = 0;
				continue;
			}

			if (*ofs == '"' || *ofs == '\'') {
				cur_quote = *ofs++;
				continue;
			}

			break;
		}

		token = MALLOC(ofs_op - op_buf + 1);
		memcpy(token, op_buf, ofs_op - op_buf);
		token[ofs_op - op_buf] = '\0';

		 
		vector_alloc_slot(strvec);
		vector_set_slot(strvec, token);
	}

	FREE(op_buf);

	if (!vector_size(strvec)) {
		free_strvec(strvec);
		return NULL;
	}

	return strvec;

err_exit:
	free_strvec(strvec);
	FREE(op_buf);
	return NULL;
}