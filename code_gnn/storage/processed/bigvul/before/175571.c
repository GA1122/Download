AMRExtractor::AMRExtractor(const sp<DataSource> &source)
 : mDataSource(source),
      mInitCheck(NO_INIT),
      mOffsetTableLength(0) {
 String8 mimeType;
 float confidence;
 if (!SniffAMR(mDataSource, &mimeType, &confidence, NULL)) {
 return;
 }

    mIsWide = (mimeType == MEDIA_MIMETYPE_AUDIO_AMR_WB);

    mMeta = new MetaData;
    mMeta->setCString(
            kKeyMIMEType, mIsWide ? MEDIA_MIMETYPE_AUDIO_AMR_WB
 : MEDIA_MIMETYPE_AUDIO_AMR_NB);

    mMeta->setInt32(kKeyChannelCount, 1);
    mMeta->setInt32(kKeySampleRate, mIsWide ? 16000 : 8000);

 off64_t offset = mIsWide ? 9 : 6;
 off64_t streamSize;
 size_t frameSize, numFrames = 0;
 int64_t duration = 0;

 if (mDataSource->getSize(&streamSize) == OK) {
 while (offset < streamSize) {
 status_t status = getFrameSizeByOffset(source, offset, mIsWide, &frameSize);
 if (status == ERROR_END_OF_STREAM) {
 break;
 } else if (status != OK) {
 return;
 }

 if ((numFrames % 50 == 0) && (numFrames / 50 < OFFSET_TABLE_LEN)) {
                CHECK_EQ(mOffsetTableLength, numFrames / 50);
                mOffsetTable[mOffsetTableLength] = offset - (mIsWide ? 9: 6);
                mOffsetTableLength ++;
 }

            offset += frameSize;
            duration += 20000;  
            numFrames ++;
 }

        mMeta->setInt64(kKeyDuration, duration);
 }

    mInitCheck = OK;
}
