pdf_process_stream(fz_context *ctx, pdf_processor *proc, pdf_csi *csi, fz_stream *stm)
{
	pdf_document *doc = csi->doc;
	pdf_lexbuf *buf = csi->buf;
	fz_cookie *cookie = csi->cookie;

	pdf_token tok = PDF_TOK_ERROR;
	int in_text_array = 0;
	int syntax_errors = 0;

	 
	pdf_clear_stack(ctx, csi);

	fz_var(in_text_array);
	fz_var(tok);

	if (cookie)
	{
		cookie->progress_max = -1;
		cookie->progress = 0;
	}

	do
	{
		fz_try(ctx)
		{
			do
			{
				 
				if (cookie)
				{
					if (cookie->abort)
					{
						tok = PDF_TOK_EOF;
						break;
					}
					cookie->progress++;
				}

				tok = pdf_lex(ctx, stm, buf);

				if (in_text_array)
				{
					switch(tok)
					{
					case PDF_TOK_CLOSE_ARRAY:
						in_text_array = 0;
						break;
					case PDF_TOK_REAL:
						pdf_array_push_drop(ctx, csi->obj, pdf_new_real(ctx, doc, buf->f));
						break;
					case PDF_TOK_INT:
						pdf_array_push_drop(ctx, csi->obj, pdf_new_int(ctx, doc, buf->i));
						break;
					case PDF_TOK_STRING:
						pdf_array_push_drop(ctx, csi->obj, pdf_new_string(ctx, doc, buf->scratch, buf->len));
						break;
					case PDF_TOK_EOF:
						break;
					case PDF_TOK_KEYWORD:
						if (buf->scratch[0] == 'T' && (buf->scratch[1] == 'w' || buf->scratch[1] == 'c') && buf->scratch[2] == 0)
						{
							int n = pdf_array_len(ctx, csi->obj);
							if (n > 0)
							{
								pdf_obj *o = pdf_array_get(ctx, csi->obj, n-1);
								if (pdf_is_number(ctx, o))
								{
									csi->stack[0] = pdf_to_real(ctx, o);
									pdf_array_delete(ctx, csi->obj, n-1);
									pdf_process_keyword(ctx, proc, csi, stm, buf->scratch);
								}
							}
						}
						 
					default:
						fz_throw(ctx, FZ_ERROR_SYNTAX, "syntax error in array");
					}
				}
				else switch (tok)
				{
				case PDF_TOK_ENDSTREAM:
				case PDF_TOK_EOF:
					tok = PDF_TOK_EOF;
					break;

				case PDF_TOK_OPEN_ARRAY:
					if (csi->obj)
					{
						pdf_drop_obj(ctx, csi->obj);
						csi->obj = NULL;
					}
					if (csi->in_text)
					{
						in_text_array = 1;
						csi->obj = pdf_new_array(ctx, doc, 4);
					}
					else
					{
						csi->obj = pdf_parse_array(ctx, doc, stm, buf);
					}
					break;

				case PDF_TOK_OPEN_DICT:
					if (csi->obj)
					{
						pdf_drop_obj(ctx, csi->obj);
						csi->obj = NULL;
					}
					csi->obj = pdf_parse_dict(ctx, doc, stm, buf);
					break;

				case PDF_TOK_NAME:
					if (csi->name[0])
					{
						pdf_drop_obj(ctx, csi->obj);
						csi->obj = NULL;
						csi->obj = pdf_new_name(ctx, doc, buf->scratch);
					}
					else
						fz_strlcpy(csi->name, buf->scratch, sizeof(csi->name));
					break;

				case PDF_TOK_INT:
					if (csi->top < nelem(csi->stack)) {
						csi->stack[csi->top] = buf->i;
						csi->top ++;
					}
					else
						fz_throw(ctx, FZ_ERROR_SYNTAX, "stack overflow");
					break;

				case PDF_TOK_REAL:
					if (csi->top < nelem(csi->stack)) {
						csi->stack[csi->top] = buf->f;
						csi->top ++;
					}
					else
						fz_throw(ctx, FZ_ERROR_SYNTAX, "stack overflow");
					break;

				case PDF_TOK_STRING:
					if (buf->len <= sizeof(csi->string))
					{
						memcpy(csi->string, buf->scratch, buf->len);
						csi->string_len = buf->len;
					}
					else
					{
						if (csi->obj)
						{
							pdf_drop_obj(ctx, csi->obj);
							csi->obj = NULL;
						}
						csi->obj = pdf_new_string(ctx, doc, buf->scratch, buf->len);
					}
					break;

				case PDF_TOK_KEYWORD:
					pdf_process_keyword(ctx, proc, csi, stm, buf->scratch);
					pdf_clear_stack(ctx, csi);
					break;

				default:
					fz_throw(ctx, FZ_ERROR_SYNTAX, "syntax error in content stream");
				}
			}
			while (tok != PDF_TOK_EOF);
		}
		fz_always(ctx)
		{
			pdf_clear_stack(ctx, csi);
		}
		fz_catch(ctx)
		{
			int caught = fz_caught(ctx);

			if (cookie)
			{
				if (caught == FZ_ERROR_TRYLATER)
				{
					if (cookie->incomplete_ok)
						cookie->incomplete++;
					else
						fz_rethrow(ctx);
				}
				else if (caught == FZ_ERROR_ABORT)
				{
					fz_rethrow(ctx);
				}
				else if (caught == FZ_ERROR_SYNTAX)
				{
					cookie->errors++;
					if (++syntax_errors >= MAX_SYNTAX_ERRORS)
					{
						fz_warn(ctx, "too many syntax errors; ignoring rest of page");
						tok = PDF_TOK_EOF;
					}
				}
				else
				{
					cookie->errors++;
					fz_warn(ctx, "unrecoverable error; ignoring rest of page");
					tok = PDF_TOK_EOF;
				}
			}
			else
			{
				if (caught == FZ_ERROR_TRYLATER)
					fz_rethrow(ctx);
				else if (caught == FZ_ERROR_ABORT)
					fz_rethrow(ctx);
				else if (caught == FZ_ERROR_SYNTAX)
				{
					if (++syntax_errors >= MAX_SYNTAX_ERRORS)
					{
						fz_warn(ctx, "too many syntax errors; ignoring rest of page");
						tok = PDF_TOK_EOF;
					}
				}
				else
				{
					fz_warn(ctx, "unrecoverable error; ignoring rest of page");
					tok = PDF_TOK_EOF;
				}
			}

			 
			in_text_array = 0;
		}
	}
	while (tok != PDF_TOK_EOF);
}
