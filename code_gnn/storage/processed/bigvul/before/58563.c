int transport_read(rdpTransport* transport, wStream* s)
{
	int status;
	int pduLength;
	int streamPosition;
	int transport_status;

	pduLength = 0;
	transport_status = 0;

	 
	streamPosition = Stream_GetPosition(s);

	if (streamPosition < 4)
	{
		status = transport_read_layer(transport, Stream_Buffer(s) + streamPosition, 4 - streamPosition);

		if (status < 0)
			return status;

		transport_status += status;

		if ((status + streamPosition) < 4)
			return transport_status;

		streamPosition += status;
	}

	 
	if (s->buffer[0] == 0x03)
	{
		 

		pduLength = (s->buffer[2] << 8) | s->buffer[3];
	}
	else if (s->buffer[0] == 0x30)
	{
		 

		if (s->buffer[1] & 0x80)
		{
			if ((s->buffer[1] & ~(0x80)) == 1)
			{
				pduLength = s->buffer[2];
				pduLength += 3;
			}
			else if ((s->buffer[1] & ~(0x80)) == 2)
			{
				pduLength = (s->buffer[2] << 8) | s->buffer[3];
				pduLength += 4;
			}
			else
			{
				fprintf(stderr, "Error reading TSRequest!\n");
			}
		}
		else
		{
			pduLength = s->buffer[1];
			pduLength += 2;
		}
	}
	else
	{
		 

		if (s->buffer[1] & 0x80)
			pduLength = ((s->buffer[1] & 0x7F) << 8) | s->buffer[2];
		else
			pduLength = s->buffer[1];
	}

	status = transport_read_layer(transport, Stream_Buffer(s) + streamPosition, pduLength - streamPosition);

	if (status < 0)
		return status;

	transport_status += status;

#ifdef WITH_DEBUG_TRANSPORT
	 
	if (streamPosition + status >= pduLength)
	{
		fprintf(stderr, "Local < Remote\n");
		winpr_HexDump(Stream_Buffer(s), pduLength);
	}
#endif

	return transport_status;
}