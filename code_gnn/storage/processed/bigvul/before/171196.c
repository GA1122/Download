void MediaPlayerService::AudioOutput::setAudioStreamType(audio_stream_type_t streamType)
{
 Mutex::Autolock lock(mLock);
 if (mAttributes == NULL) {
        mStreamType = streamType;
 }
}
