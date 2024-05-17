DRMSource::DRMSource(const sp<MediaSource> &mediaSource,
 const sp<DecryptHandle> &decryptHandle,
 DrmManagerClient *managerClient,
 int32_t trackId, DrmBuffer *ipmpBox)
 : mOriginalMediaSource(mediaSource),
      mDecryptHandle(decryptHandle),
      mDrmManagerClient(managerClient),
      mTrackId(trackId),
      mNALLengthSize(0),
      mWantsNALFragments(false) {
    CHECK(mDrmManagerClient);
    mDrmManagerClient->initializeDecryptUnit(
            mDecryptHandle, trackId, ipmpBox);

 const char *mime;
 bool success = getFormat()->findCString(kKeyMIMEType, &mime);
    CHECK(success);

 if (!strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_AVC)) {
 uint32_t type;
 const void *data;
 size_t size;
        CHECK(getFormat()->findData(kKeyAVCC, &type, &data, &size));

 const uint8_t *ptr = (const uint8_t *)data;

        CHECK(size >= 7);
        CHECK_EQ(ptr[0], 1);  

        mNALLengthSize = 1 + (ptr[4] & 3);
 }
}
