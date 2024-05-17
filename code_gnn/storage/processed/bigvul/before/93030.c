rdp_recv(uint8 * type)
{
	RD_BOOL is_fastpath;
	static STREAM rdp_s;
	uint16 length;

	while (1)
	{
		 
		if ((rdp_s == NULL) || (g_next_packet >= rdp_s->end) || (g_next_packet == NULL))
		{
			rdp_s = sec_recv(&is_fastpath);
			if (rdp_s == NULL)
				return NULL;

			if (is_fastpath == True)
			{
				 
				process_ts_fp_updates(rdp_s);
				continue;
			}

			g_next_packet = rdp_s->p;
		}
		else
		{
			rdp_s->p = g_next_packet;
		}

		 
		if (rdp_ts_in_share_control_header(rdp_s, type, &length) == False)
			continue;

		break;
	}

	logger(Protocol, Debug, "rdp_recv(), RDP packet #%d, type 0x%x", ++g_packetno, *type);

	g_next_packet += length;
	return rdp_s;
}
