int transport_check_fds(rdpTransport** ptransport)
{
	int pos;
	int status;
	UINT16 length;
	int recv_status;
	wStream* received;
	rdpTransport* transport = *ptransport;

#ifdef _WIN32
	WSAResetEvent(transport->TcpIn->wsa_event);
#endif
	ResetEvent(transport->ReceiveEvent);

	status = transport_read_nonblocking(transport);

	if (status < 0)
		return status;

	while ((pos = Stream_GetPosition(transport->ReceiveBuffer)) > 0)
	{
		Stream_SetPosition(transport->ReceiveBuffer, 0);

		if (tpkt_verify_header(transport->ReceiveBuffer))  
		{
			 
			if (pos <= 4)
			{
				Stream_SetPosition(transport->ReceiveBuffer, pos);
				return 0;
			}

			length = tpkt_read_header(transport->ReceiveBuffer);
		}
		else if (nla_verify_header(transport->ReceiveBuffer))
		{
			 

			 
			if (pos <= 4)
			{
				Stream_SetPosition(transport->ReceiveBuffer, pos);
				return 0;
			}

			 
			length = nla_header_length(transport->ReceiveBuffer);

			if (pos < length)
			{
				Stream_SetPosition(transport->ReceiveBuffer, pos);
				return 0;
			}

			length = nla_read_header(transport->ReceiveBuffer);
		}
		else  
		{
			 
			if (pos <= 2)
			{
				Stream_SetPosition(transport->ReceiveBuffer, pos);
				return 0;
			}

			 
			length = fastpath_header_length(transport->ReceiveBuffer);

			if (pos < length)
			{
				Stream_SetPosition(transport->ReceiveBuffer, pos);
				return 0;
			}

			length = fastpath_read_header(NULL, transport->ReceiveBuffer);
		}

		if (length == 0)
		{
			fprintf(stderr, "transport_check_fds: protocol error, not a TPKT or Fast Path header.\n");
			winpr_HexDump(Stream_Buffer(transport->ReceiveBuffer), pos);
			return -1;
		}

		if (pos < length)
		{
			Stream_SetPosition(transport->ReceiveBuffer, pos);
			return 0;  
		}

		received = transport->ReceiveBuffer;
		transport->ReceiveBuffer = StreamPool_Take(transport->ReceivePool, 0);

		Stream_SetPosition(received, length);
		Stream_SealLength(received);
		Stream_SetPosition(received, 0);

		recv_status = transport->ReceiveCallback(transport, received, transport->ReceiveExtra);

		Stream_Release(received);

		if (recv_status < 0)
			status = -1;

		if (status < 0)
			return status;

		 
		transport = *ptransport;
	}

	return 0;
}