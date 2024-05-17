dump_cfg_strarray_oneline(ServerOpCodes code, u_int count, char **vals)
{
	u_int i;

	if (count <= 0 && code != sAuthenticationMethods)
		return;
	printf("%s", lookup_opcode_name(code));
	for (i = 0; i < count; i++)
		printf(" %s",  vals[i]);
	if (code == sAuthenticationMethods && count == 0)
		printf(" any");
	printf("\n");
}
