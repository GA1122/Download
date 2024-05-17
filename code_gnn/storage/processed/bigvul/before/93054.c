rdpdr_send_client_capability_response(void)
{
	 
	STREAM s;
	s = channel_init(rdpdr_channel, 0x50);

	out_uint16_le(s, RDPDR_CTYP_CORE);	 
	out_uint16_le(s, PAKID_CORE_CLIENT_CAPABILITY);	 
	out_uint16_le(s, 5);	 
	out_uint16_le(s, 0);	 

	out_uint16_le(s, CAP_GENERAL_TYPE);	 
	out_uint16_le(s, 0x28);	 
	out_uint32_le(s, GENERAL_CAPABILITY_VERSION_01);	 
	out_uint32_le(s, 0);	 
	out_uint32_le(s, 0);	 
	out_uint16_le(s, 1);	 
	out_uint16_le(s, 5);	 
	out_uint32_le(s, ALL_RDPDR_IRP_MJ);	 
	out_uint32_le(s, 0);	 
	out_uint32_le(s, RDPDR_DEVICE_REMOVE_PDUS | RDPDR_CLIENT_DISPLAY_NAME_PDU);	 
	out_uint32_le(s, 0);	 
	out_uint32_le(s, 0);	 

	out_uint16_le(s, CAP_PRINTER_TYPE);	 
	out_uint16_le(s, 8);	 
	out_uint32_le(s, PRINT_CAPABILITY_VERSION_01);	 

	out_uint16_le(s, CAP_PORT_TYPE);	 
	out_uint16_le(s, 8);	 
	out_uint32_le(s, PORT_CAPABILITY_VERSION_01);	 

	out_uint16_le(s, CAP_DRIVE_TYPE);	 
	out_uint16_le(s, 8);	 
	out_uint32_le(s, DRIVE_CAPABILITY_VERSION_02);	 

	out_uint16_le(s, CAP_SMARTCARD_TYPE);	 
	out_uint16_le(s, 8);	 
	out_uint32_le(s, SMARTCARD_CAPABILITY_VERSION_01);	 

	s_mark_end(s);
	channel_send(s, rdpdr_channel);
}
