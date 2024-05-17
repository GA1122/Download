static void Sp_split_string(js_State *J)
{
	const char *str = checkstring(J, 0);
	const char *sep = js_tostring(J, 1);
	int limit = js_isdefined(J, 2) ? js_tointeger(J, 2) : 1 << 30;
	int i, n;

	js_newarray(J);

	n = strlen(sep);

	 
	if (n == 0) {
		Rune rune;
		for (i = 0; *str && i < limit; ++i) {
			n = chartorune(&rune, str);
			js_pushlstring(J, str, n);
			js_setindex(J, -2, i);
			str += n;
		}
		return;
	}

	for (i = 0; str && i < limit; ++i) {
		const char *s = strstr(str, sep);
		if (s) {
			js_pushlstring(J, str, s-str);
			js_setindex(J, -2, i);
			str = s + n;
		} else {
			js_pushstring(J, str);
			js_setindex(J, -2, i);
			str = NULL;
		}
	}
}