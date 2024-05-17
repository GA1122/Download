status_t MediaPlayerService::Client::reset()
{
    ALOGV("[%d] reset", mConnId);
    mRetransmitEndpointValid = false;
    sp<MediaPlayerBase> p = getPlayer();
 if (p == 0) return UNKNOWN_ERROR;
 return p->reset();
}
