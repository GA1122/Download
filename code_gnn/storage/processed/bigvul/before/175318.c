sp<IMediaSource> MPEG4Extractor::getTrack(size_t index) {
 status_t err;
 if ((err = readMetaData()) != OK) {
 return NULL;
 }

 Track *track = mFirstTrack;
 while (index > 0) {
 if (track == NULL) {
 return NULL;
 }

        track = track->next;
 --index;
 }

 if (track == NULL) {
 return NULL;
 }


 Trex *trex = NULL;
 int32_t trackId;
 if (track->meta->findInt32(kKeyTrackID, &trackId)) {
 for (size_t i = 0; i < mTrex.size(); i++) {
 Trex *t = &mTrex.editItemAt(i);
 if (t->track_ID == (uint32_t) trackId) {
                trex = t;
 break;
 }
 }
 } else {
        ALOGE("b/21657957");
 return NULL;
 }

    ALOGV("getTrack called, pssh: %zu", mPssh.size());

 const char *mime;
 if (!track->meta->findCString(kKeyMIMEType, &mime)) {
 return NULL;
 }

 if (!strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_AVC)) {
 uint32_t type;
 const void *data;
 size_t size;
 if (!track->meta->findData(kKeyAVCC, &type, &data, &size)) {
 return NULL;
 }

 const uint8_t *ptr = (const uint8_t *)data;

 if (size < 7 || ptr[0] != 1) {  
 return NULL;
 }
 } else if (!strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_HEVC)) {
 uint32_t type;
 const void *data;
 size_t size;
 if (!track->meta->findData(kKeyHVCC, &type, &data, &size)) {
 return NULL;
 }

 const uint8_t *ptr = (const uint8_t *)data;

 if (size < 22 || ptr[0] != 1) {  
 return NULL;
 }
 }

    sp<MPEG4Source> source = new MPEG4Source(this,
            track->meta, mDataSource, track->timescale, track->sampleTable,
            mSidxEntries, trex, mMoofOffset);
 if (source->init() != OK) {
 return NULL;
 }
 return source;
}
