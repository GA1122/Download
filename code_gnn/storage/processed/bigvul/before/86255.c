optlen(const u_int8_t *opt, unsigned int offset)
{
	 
	if (opt[offset] <= TCPOPT_NOP || opt[offset+1] == 0)
		return 1;
	else
		return opt[offset+1];
}
