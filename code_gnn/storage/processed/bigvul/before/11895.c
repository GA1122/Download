static void jsP_warning(js_State *J, const char *fmt, ...)
{
	va_list ap;
	char buf[512];
	char msg[256];

	va_start(ap, fmt);
	vsnprintf(msg, sizeof msg, fmt, ap);
	va_end(ap);

	snprintf(buf, sizeof buf, "%s:%d: warning: %s", J->filename, J->lexline, msg);
	js_report(J, buf);
}
