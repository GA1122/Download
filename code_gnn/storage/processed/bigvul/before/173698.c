sp<MetaData> AudioSource::getFormat() {
 Mutex::Autolock autoLock(mLock);
 if (mInitCheck != OK) {
 return 0;
 }

    sp<MetaData> meta = new MetaData;
    meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_RAW);
    meta->setInt32(kKeySampleRate, mSampleRate);
    meta->setInt32(kKeyChannelCount, mRecord->channelCount());
    meta->setInt32(kKeyMaxInputSize, kMaxBufferSize);

 return meta;
}
