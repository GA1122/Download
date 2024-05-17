int transport_read_layer(rdpTransport* transport, UINT8* data, int bytes)
{
	int read = 0;
	int status = -1;

	while (read < bytes)
	{
		if (transport->layer == TRANSPORT_LAYER_TLS)
			status = tls_read(transport->TlsIn, data + read, bytes - read);
		else if (transport->layer == TRANSPORT_LAYER_TCP)
			status = tcp_read(transport->TcpIn, data + read, bytes - read);
		else if (transport->layer == TRANSPORT_LAYER_TSG)
			status = tsg_read(transport->tsg, data + read, bytes - read);

		 

		if (!transport->blocking)
			return status;

		if (status < 0)
			return status;

		read += status;

		if (status == 0)
		{
			 
			USleep(transport->SleepInterval);
		}
	}

	return read;
}