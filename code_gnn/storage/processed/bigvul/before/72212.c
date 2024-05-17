dump_cfg_int(ServerOpCodes code, int val)
{
	printf("%s %d\n", lookup_opcode_name(code), val);
}
