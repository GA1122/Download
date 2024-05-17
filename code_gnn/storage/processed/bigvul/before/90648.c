static void Rp_toString(js_State *J)
{
	js_Regexp *re;
	char *out;

	re = js_toregexp(J, 0);

	out = js_malloc(J, strlen(re->source) + 6);  
	strcpy(out, "/");
	strcat(out, re->source);
	strcat(out, "/");
	if (re->flags & JS_REGEXP_G) strcat(out, "g");
	if (re->flags & JS_REGEXP_I) strcat(out, "i");
	if (re->flags & JS_REGEXP_M) strcat(out, "m");

	if (js_try(J)) {
		js_free(J, out);
		js_throw(J);
	}
	js_pop(J, 0);
	js_pushstring(J, out);
	js_endtry(J);
	js_free(J, out);
}