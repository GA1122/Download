static void __init kuser_get_tls_init(unsigned long vectors)
{
	 
	if (tls_emu || has_tls_reg)
		memcpy((void *)vectors + 0xfe0, (void *)vectors + 0xfe8, 4);
}
