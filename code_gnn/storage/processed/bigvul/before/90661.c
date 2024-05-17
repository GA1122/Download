static void Sp_replace_string(js_State *J)
{
	const char *source, *needle, *s, *r;
	js_Buffer *sb = NULL;
	int n;

	source = checkstring(J, 0);
	needle = js_tostring(J, 1);

	s = strstr(source, needle);
	if (!s) {
		js_copy(J, 0);
		return;
	}
	n = strlen(needle);

	if (js_iscallable(J, 2)) {
		js_copy(J, 2);
		js_pushundefined(J);
		js_pushlstring(J, s, n);  
		js_pushnumber(J, s - source);  
		js_copy(J, 0);  
		js_call(J, 3);
		r = js_tostring(J, -1);
		js_putm(J, &sb, source, s);
		js_puts(J, &sb, r);
		js_puts(J, &sb, s + n);
		js_putc(J, &sb, 0);
		js_pop(J, 1);
	} else {
		r = js_tostring(J, 2);
		js_putm(J, &sb, source, s);
		while (*r) {
			if (*r == '$') {
				switch (*(++r)) {
				case 0: --r;  
				 
				case '$': js_putc(J, &sb, '$'); break;
				case '&': js_putm(J, &sb, s, s + n); break;
				case '`': js_putm(J, &sb, source, s); break;
				case '\'': js_puts(J, &sb, s + n); break;
				default: js_putc(J, &sb, '$'); js_putc(J, &sb, *r); break;
				}
				++r;
			} else {
				js_putc(J, &sb, *r++);
			}
		}
		js_puts(J, &sb, s + n);
		js_putc(J, &sb, 0);
	}

	if (js_try(J)) {
		js_free(J, sb);
		js_throw(J);
	}
	js_pushstring(J, sb ? sb->s : "");
	js_endtry(J);
	js_free(J, sb);
}