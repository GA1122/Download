void MyOggExtractor::parseFileMetaData() {
    mFileMeta = new MetaData;
    mFileMeta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_CONTAINER_OGG);

 for (int i = 0; i < mVc.comments; ++i) {
 const char *comment = mVc.user_comments[i];
 size_t commentLength = mVc.comment_lengths[i];
        parseVorbisComment(mFileMeta, comment, commentLength);
 }
}
