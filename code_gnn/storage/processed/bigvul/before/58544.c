static BOOL peer_recv_data_pdu(freerdp_peer* client, wStream* s)
{
	BYTE type;
	UINT16 length;
	UINT32 share_id;
	BYTE compressed_type;
	UINT16 compressed_len;

	if (!rdp_read_share_data_header(s, &length, &type, &share_id, &compressed_type, &compressed_len))
		return FALSE;

	switch (type)
	{
		case DATA_PDU_TYPE_SYNCHRONIZE:
			if (!rdp_recv_client_synchronize_pdu(client->context->rdp, s))
				return FALSE;
			break;

		case DATA_PDU_TYPE_CONTROL:
			if (!rdp_server_accept_client_control_pdu(client->context->rdp, s))
				return FALSE;
			break;

		case DATA_PDU_TYPE_INPUT:
			if (!input_recv(client->context->rdp->input, s))
				return FALSE;
			break;

		case DATA_PDU_TYPE_BITMAP_CACHE_PERSISTENT_LIST:
			 
			break;

		case DATA_PDU_TYPE_FONT_LIST:

			if (!rdp_server_accept_client_font_list_pdu(client->context->rdp, s))
				return FALSE;

			if (!client->connected)
			{
				 

				IFCALLRET(client->PostConnect, client->connected, client);

				if (!client->connected)
					return FALSE;
			}

			if (!client->activated)
			{
				 
			
				IFCALLRET(client->Activate, client->activated, client);

				if (!client->activated)
					return FALSE;
			}

			break;

		case DATA_PDU_TYPE_SHUTDOWN_REQUEST:
			mcs_send_disconnect_provider_ultimatum(client->context->rdp->mcs);
			return FALSE;

		case DATA_PDU_TYPE_FRAME_ACKNOWLEDGE:
			if(Stream_GetRemainingLength(s) < 4)
				return FALSE;
			Stream_Read_UINT32(s, client->ack_frame_id);
			IFCALL(client->update->SurfaceFrameAcknowledge, client->update->context, client->ack_frame_id);
			break;

		case DATA_PDU_TYPE_REFRESH_RECT:
			if (!update_read_refresh_rect(client->update, s))
				return FALSE;
			break;

		case DATA_PDU_TYPE_SUPPRESS_OUTPUT:
			if (!update_read_suppress_output(client->update, s))
				return FALSE;
			break;

		default:
			fprintf(stderr, "Data PDU type %d\n", type);
			break;
	}

	return TRUE;
}