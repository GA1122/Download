sp<MetaData> OMXCodec::getFormat() {
 Mutex::Autolock autoLock(mLock);

 return mOutputFormat;
}
