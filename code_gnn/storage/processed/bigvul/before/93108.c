sec_out_mcs_connect_initial_pdu(STREAM s, uint32 selected_protocol)
{
	int length = 162 + 76 + 12 + 4 + (g_dpi > 0 ? 18 : 0);
	unsigned int i;
	uint32 rdpversion = RDP_40;
	uint16 capflags = RNS_UD_CS_SUPPORT_ERRINFO_PDU;
	uint16 colorsupport = RNS_UD_24BPP_SUPPORT | RNS_UD_16BPP_SUPPORT | RNS_UD_32BPP_SUPPORT;
	uint32 physwidth, physheight, desktopscale, devicescale;

	logger(Protocol, Debug, "%s()", __func__);

	if (g_rdp_version >= RDP_V5)
		rdpversion = RDP_50;

	if (g_num_channels > 0)
		length += g_num_channels * 12 + 8;

	 
	out_uint16_be(s, 5);
	out_uint16_be(s, 0x14);
	out_uint8(s, 0x7c);
	out_uint16_be(s, 1);

	out_uint16_be(s, (length | 0x8000));	 

	out_uint16_be(s, 8);	 
	out_uint16_be(s, 16);
	out_uint8(s, 0);
	out_uint16_le(s, 0xc001);
	out_uint8(s, 0);

	out_uint32_le(s, 0x61637544);	 
	out_uint16_be(s, ((length - 14) | 0x8000));	 

	 
	out_uint16_le(s, CS_CORE);	 
	out_uint16_le(s, 216 + (g_dpi > 0 ? 18 : 0));	 
	out_uint32_le(s, rdpversion);	 
	out_uint16_le(s, g_requested_session_width);	 
	out_uint16_le(s, g_requested_session_height);	 
	out_uint16_le(s, RNS_UD_COLOR_8BPP);	 
	out_uint16_le(s, RNS_UD_SAS_DEL);	 
	out_uint32_le(s, g_keylayout);	 
	out_uint32_le(s, 2600);	 

	 
	out_utf16s_padded(s, g_hostname, 32, 0x00);

	out_uint32_le(s, g_keyboard_type);	 
	out_uint32_le(s, g_keyboard_subtype);	 
	out_uint32_le(s, g_keyboard_functionkeys);	 
	out_uint8s(s, 64);	 
	out_uint16_le(s, RNS_UD_COLOR_8BPP);	 
	out_uint16_le(s, 1);	 
	out_uint32_le(s, 0);	 

	 
	out_uint16_le(s, MIN(g_server_depth, 24));
	if (g_server_depth == 32)
		capflags |= RNS_UD_CS_WANT_32BPP_SESSION;

	out_uint16_le(s, colorsupport);	 
	out_uint16_le(s, capflags);	 
	out_uint8s(s, 64);	 
	out_uint8(s, 0);	 
	out_uint8(s, 0);	 
	out_uint32_le(s, selected_protocol);	 
	if (g_dpi > 0)
	{
		 
		utils_calculate_dpi_scale_factors(g_requested_session_width,
						  g_requested_session_height, g_dpi, &physwidth,
						  &physheight, &desktopscale, &devicescale);
		out_uint32_le(s, physwidth);	 
		out_uint32_le(s, physheight);	 
		out_uint16_le(s, ORIENTATION_LANDSCAPE);	 
		out_uint32_le(s, desktopscale);	 
		out_uint32_le(s, devicescale);	 
	}

	 
	uint32 cluster_flags = 0;
	out_uint16_le(s, CS_CLUSTER);	 
	out_uint16_le(s, 12);	 

	cluster_flags |= SEC_CC_REDIRECTION_SUPPORTED;
	cluster_flags |= (SEC_CC_REDIRECT_VERSION_3 << 2);

	if (g_console_session || g_redirect_session_id != 0)
		cluster_flags |= SEC_CC_REDIRECT_SESSIONID_FIELD_VALID;

	out_uint32_le(s, cluster_flags);
	out_uint32(s, g_redirect_session_id);

	 
	out_uint16_le(s, CS_SECURITY);	 
	out_uint16_le(s, 12);	 
	out_uint32_le(s, g_encryption ? 0x3 : 0);	 
	out_uint32(s, 0);	 

	 
	logger(Protocol, Debug, "sec_out_mcs_data(), g_num_channels is %d", g_num_channels);
	if (g_num_channels > 0)
	{
		out_uint16_le(s, CS_NET);	 
		out_uint16_le(s, g_num_channels * 12 + 8);	 
		out_uint32_le(s, g_num_channels);	 
		for (i = 0; i < g_num_channels; i++)
		{
			logger(Protocol, Debug, "sec_out_mcs_data(), requesting channel %s",
			       g_channels[i].name);
			out_uint8a(s, g_channels[i].name, 8);
			out_uint32_be(s, g_channels[i].flags);
		}
	}

	s_mark_end(s);
}
