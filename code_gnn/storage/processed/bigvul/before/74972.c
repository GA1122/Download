static UINT dvcman_close_channel(IWTSVirtualChannelManager* pChannelMgr,
                                 UINT32 ChannelId)
{
	DVCMAN_CHANNEL* channel;
	UINT error = CHANNEL_RC_OK;
	DVCMAN* dvcman = (DVCMAN*) pChannelMgr;
	channel = (DVCMAN_CHANNEL*) dvcman_find_channel_by_id(pChannelMgr, ChannelId);

	if (!channel)
	{
		 
		return CHANNEL_RC_OK;
	}

	ArrayList_Remove(dvcman->channels, channel);
	return error;
}
