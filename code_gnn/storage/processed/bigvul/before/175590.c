bool NuMediaExtractor::getCachedDuration(
 int64_t *durationUs, bool *eos) const {
 Mutex::Autolock autoLock(mLock);

 int64_t bitrate;
 if (mIsWidevineExtractor) {
        sp<WVMExtractor> wvmExtractor =
 static_cast<WVMExtractor *>(mImpl.get());

 status_t finalStatus;
 *durationUs = wvmExtractor->getCachedDurationUs(&finalStatus);
 *eos = (finalStatus != OK);
 return true;
 } else if ((mDataSource->flags() & DataSource::kIsCachingDataSource)
 && getTotalBitrate(&bitrate)) {
        sp<NuCachedSource2> cachedSource =
 static_cast<NuCachedSource2 *>(mDataSource.get());

 status_t finalStatus;
 size_t cachedDataRemaining =
            cachedSource->approxDataRemaining(&finalStatus);

 *durationUs = cachedDataRemaining * 8000000ll / bitrate;
 *eos = (finalStatus != OK);
 return true;
 }

 return false;
}
