rdpTransport* transport_new(rdpSettings* settings)
{
	rdpTransport* transport;

	transport = (rdpTransport*) malloc(sizeof(rdpTransport));

	if (transport != NULL)
	{
		ZeroMemory(transport, sizeof(rdpTransport));

		transport->TcpIn = tcp_new(settings);

		transport->settings = settings;

		 
		transport->SleepInterval = 100;

		transport->ReceivePool = StreamPool_New(TRUE, BUFFER_SIZE);

		 
		transport->ReceiveBuffer = StreamPool_Take(transport->ReceivePool, 0);
		transport->ReceiveEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		transport->connectedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		transport->blocking = TRUE;

		transport->ReadMutex = CreateMutex(NULL, FALSE, NULL);
		transport->WriteMutex = CreateMutex(NULL, FALSE, NULL);

		transport->layer = TRANSPORT_LAYER_TCP;
	}

	return transport;
}