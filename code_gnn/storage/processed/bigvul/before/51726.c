wv_csp_discriminator(tvbuff_t *tvb, guint32 offset)
{
	guint32 magic_1 = tvb_get_ntohl(tvb, offset + 0);
	guint16 magic_2 = tvb_get_ntohs(tvb, offset + 4);

	if (magic_1 == 0xFE050331 && magic_2 == 0x2e30)
		{
			 
			return &decode_wv_cspc_10;
		}
	else if (magic_1 == 0xC9050331 && magic_2 == 0x2e31)
		{
			 
			return &decode_wv_cspc_11;
		}
	else if (magic_1 == 0xC9080331 && magic_2 == 0x2e32)
		{
			 
			return &decode_wv_cspc_12;
		}
	else if ( magic_1 == 0xC90B0331 && magic_2 == 0x2E33)
		{
			 
			return &decode_wv_cspc_13;
		}


	 
	return &decode_wv_cspc_12;
}
