dump_cfg_oct(ServerOpCodes code, int val)
{
	printf("%s 0%o\n", lookup_opcode_name(code), val);
}
