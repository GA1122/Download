static void ipmi_debug_msg(const char *title, unsigned char *data,
			   unsigned int len)
{
	int i, pos;
	char buf[100];

	pos = snprintf(buf, sizeof(buf), "%s: ", title);
	for (i = 0; i < len; i++)
		pos += snprintf(buf + pos, sizeof(buf) - pos,
				" %2.2x", data[i]);
	pr_debug("%s\n", buf);
}
