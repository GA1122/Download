void MediaPlayerService::AudioOutput::setAudioAttributes(const audio_attributes_t * attributes) {
 Mutex::Autolock lock(mLock);
 if (attributes == NULL) {
        free(mAttributes);
        mAttributes = NULL;
 } else {
 if (mAttributes == NULL) {
            mAttributes = (audio_attributes_t *) calloc(1, sizeof(audio_attributes_t));
 }
        memcpy(mAttributes, attributes, sizeof(audio_attributes_t));
        mStreamType = audio_attributes_to_stream_type(attributes);
 }
}
