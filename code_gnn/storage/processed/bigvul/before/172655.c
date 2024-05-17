status_t MediaPlayer::doSetRetransmitEndpoint(const sp<IMediaPlayer>& player) {
 Mutex::Autolock _l(mLock);

 if (player == NULL) {
 return UNKNOWN_ERROR;
 }

 if (mRetransmitEndpointValid) {
 return player->setRetransmitEndpoint(&mRetransmitEndpoint);
 }

 return OK;
}
