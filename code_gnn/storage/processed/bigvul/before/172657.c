status_t MediaPlayer::getAudioStreamType(audio_stream_type_t *type)
{
    ALOGV("getAudioStreamType");
 Mutex::Autolock _l(mLock);
 *type = mStreamType;
 return OK;
}
