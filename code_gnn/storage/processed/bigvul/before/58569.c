int transport_write(rdpTransport* transport, wStream* s)
{
	int length;
	int status = -1;

	WaitForSingleObject(transport->WriteMutex, INFINITE);

	length = Stream_GetPosition(s);
	Stream_SetPosition(s, 0);

#ifdef WITH_DEBUG_TRANSPORT
	if (length > 0)
	{
		fprintf(stderr, "Local > Remote\n");
		winpr_HexDump(Stream_Buffer(s), length);
	}
#endif

	while (length > 0)
	{
		if (transport->layer == TRANSPORT_LAYER_TLS)
			status = tls_write(transport->TlsOut, Stream_Pointer(s), length);
		else if (transport->layer == TRANSPORT_LAYER_TCP)
			status = tcp_write(transport->TcpOut, Stream_Pointer(s), length);
		else if (transport->layer == TRANSPORT_LAYER_TSG)
			status = tsg_write(transport->tsg, Stream_Pointer(s), length);

		if (status < 0)
			break;  

		if (status == 0)
		{
			 
			if (!transport->blocking)
			{
				 
				if (transport_read_nonblocking(transport) > 0)
					SetEvent(transport->ReceiveEvent);
			}

			if (transport->layer == TRANSPORT_LAYER_TLS)
				tls_wait_write(transport->TlsOut);
			else if (transport->layer == TRANSPORT_LAYER_TCP)
				tcp_wait_write(transport->TcpOut);
			else
				USleep(transport->SleepInterval);
		}

		length -= status;
		Stream_Seek(s, status);
	}

	if (status < 0)
	{
		 
		transport->layer = TRANSPORT_LAYER_CLOSED;
	}

	if (s->pool)
		Stream_Release(s);

	ReleaseMutex(transport->WriteMutex);

	return status;
}
