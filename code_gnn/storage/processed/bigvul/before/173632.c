DRMExtractor::DRMExtractor(const sp<DataSource> &source, const char* mime)
 : mDataSource(source),
      mDecryptHandle(NULL),
      mDrmManagerClient(NULL) {
    mOriginalExtractor = MediaExtractor::Create(source, mime);
    mOriginalExtractor->setDrmFlag(true);
    mOriginalExtractor->getMetaData()->setInt32(kKeyIsDRM, 1);

    source->getDrmInfo(mDecryptHandle, &mDrmManagerClient);
}
