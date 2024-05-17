status_t MediaPlayerService::Client::setAudioStreamType(audio_stream_type_t type)
{
    ALOGV("[%d] setAudioStreamType(%d)", mConnId, type);
 Mutex::Autolock l(mLock);
 if (mAudioOutput != 0) mAudioOutput->setAudioStreamType(type);
 return NO_ERROR;
}
