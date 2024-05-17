rdpsnd_queue_clear(void)
{
	struct audio_packet *packet;

	 
	while (queue_pending != queue_hi)
	{
		packet = &packet_queue[queue_pending];
		xfree(packet->s.data);
		queue_pending = (queue_pending + 1) % MAX_QUEUE;
	}

	 
	queue_pending = queue_lo = queue_hi = 0;
}
