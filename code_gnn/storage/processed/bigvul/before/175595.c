status_t NuMediaExtractor::getTrackFormat(
 size_t index, sp<AMessage> *format, uint32_t flags) const {
 Mutex::Autolock autoLock(mLock);

 *format = NULL;

 if (mImpl == NULL) {
 return -EINVAL;
 }

 if (index >= mImpl->countTracks()) {
 return -ERANGE;
 }

    sp<MetaData> meta = mImpl->getTrackMetaData(index, flags);
 int32_t trackID;
 if (meta != NULL && !meta->findInt32(kKeyTrackID, &trackID)) {
        meta->setInt32(kKeyTrackID, (int32_t)index + 1);
 }
 return convertMetaDataToMessage(meta, format);
}
