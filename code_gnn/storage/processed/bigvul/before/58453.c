server_inline (server *serv, char *line, int len)
{
	char *utf_line_allocated = NULL;

	 
	if (serv->using_irc ||				 
		(serv->encoding == NULL && prefs.utf8_locale) ||  
	    (serv->encoding != NULL &&				 
		 (g_ascii_strcasecmp (serv->encoding, "UTF8") == 0 ||
		  g_ascii_strcasecmp (serv->encoding, "UTF-8") == 0)))
	{
		 

		utf_line_allocated = text_validate (&line, &len);

	} else
	{
		 

		const char *encoding = NULL;

		if (serv->encoding != NULL)
			encoding = serv->encoding;
		else
			g_get_charset (&encoding);

		if (encoding != NULL)
		{
			char *conv_line;  
			int conv_len;  
			gsize utf_len;
			gsize read_len;
			GError *err;
			gboolean retry;

			conv_line = g_malloc (len + 1);
			memcpy (conv_line, line, len);
			conv_line[len] = 0;
			conv_len = len;

			 
			if (serv->using_cp1255)
				conv_len++;

			do
			{
				err = NULL;
				retry = FALSE;
				utf_line_allocated = g_convert_with_fallback (conv_line, conv_len, "UTF-8", encoding, "?", &read_len, &utf_len, &err);
				if (err != NULL)
				{
					if (err->code == G_CONVERT_ERROR_ILLEGAL_SEQUENCE && conv_len > (read_len + 1))
					{
						 
						memmove (conv_line + read_len, conv_line + read_len + 1, conv_len - read_len -1);
						conv_len--;
						retry = TRUE;
					}
					g_error_free (err);
				}
			} while (retry);

			g_free (conv_line);

			 
			if (utf_line_allocated != NULL)
			{
				line = utf_line_allocated;
				len = utf_len;
				if (serv->using_cp1255 && len > 0)
					len--;
			}
			else
			{
				 
				utf_line_allocated = text_validate (&line, &len);
			}
		}
	}

	fe_add_rawlog (serv, line, len, FALSE);

	 
	serv->p_inline (serv, line, len);

	if (utf_line_allocated != NULL)  
		g_free (utf_line_allocated);
}
