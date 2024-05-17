status_t MyOggExtractor::init() {
    mMeta = new MetaData;
    mMeta->setCString(kKeyMIMEType, mMimeType);

 status_t err;
 MediaBuffer *packet;
 for (size_t i = 0; i < mNumHeaders; ++i) {
 if ((err = _readNextPacket(&packet,   false)) != OK) {
 return err;
 }
        ALOGV("read packet of size %zu\n", packet->range_length());
        err = verifyHeader(packet,   i * 2 + 1);
        packet->release();
        packet = NULL;
 if (err != OK) {
 return err;
 }
 }

    mFirstDataOffset = mOffset + mCurrentPageSize;

 off64_t size;
 uint64_t lastGranulePosition;
 if (!(mSource->flags() & DataSource::kIsCachingDataSource)
 && mSource->getSize(&size) == OK
 && findPrevGranulePosition(size, &lastGranulePosition) == OK) {

 int64_t durationUs = getTimeUsOfGranule(lastGranulePosition);

        mMeta->setInt64(kKeyDuration, durationUs);

        buildTableOfContents();
 }

 return OK;
}
