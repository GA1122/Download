static inline void efx_channel_processed(struct efx_channel *channel)
{
	 
	channel->work_pending = false;
	smp_wmb();

	efx_nic_eventq_read_ack(channel);
}
