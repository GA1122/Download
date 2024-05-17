dump_cfg_string(ServerOpCodes code, const char *val)
{
	if (val == NULL)
		return;
	printf("%s %s\n", lookup_opcode_name(code),
	    val == NULL ? "none" : val);
}
