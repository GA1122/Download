size_t NuMediaExtractor::countTracks() const {
 Mutex::Autolock autoLock(mLock);

 return mImpl == NULL ? 0 : mImpl->countTracks();
}
