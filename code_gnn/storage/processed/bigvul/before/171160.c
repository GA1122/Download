status_t MediaPlayerService::AudioOutput::getFramesWritten(uint32_t *frameswritten) const
{
 Mutex::Autolock lock(mLock);
 if (mTrack == 0) return NO_INIT;
 ExtendedTimestamp ets;
 status_t status = mTrack->getTimestamp(&ets);
 if (status == OK || status == WOULD_BLOCK) {
 *frameswritten = (uint32_t)ets.mPosition[ExtendedTimestamp::LOCATION_CLIENT];
 }
 return status;
}
