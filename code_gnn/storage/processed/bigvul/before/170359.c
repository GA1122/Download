sp<MetaData> MPEG4Source::getFormat() {
 Mutex::Autolock autoLock(mLock);

 return mFormat;
}
