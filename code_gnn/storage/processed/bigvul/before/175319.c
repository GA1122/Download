sp<MetaData> MPEG4Extractor::getTrackMetaData(
 size_t index, uint32_t flags) {
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

 if ((flags & kIncludeExtensiveMetaData)
 && !track->includes_expensive_metadata) {
        track->includes_expensive_metadata = true;

 const char *mime;
        CHECK(track->meta->findCString(kKeyMIMEType, &mime));
 if (!strncasecmp("video/", mime, 6)) {
 if (!strcmp(mime, MEDIA_MIMETYPE_VIDEO_MPEG2)) {
 off64_t offset;
 size_t size;
 if (track->sampleTable->getMetaDataForSample(
 0  , &offset, &size, NULL  ) == OK) {
 if (size > kMaxTrackHeaderSize) {
                        size = kMaxTrackHeaderSize;
 }
 uint8_t header[kMaxTrackHeaderSize];
 if (mDataSource->readAt(offset, &header, size) == (ssize_t)size) {
                        track->meta->setData(kKeyStreamHeader, 'mdat', header, size);
 }
 }
 }

 if (mMoofOffset > 0) {
 int64_t duration;
 if (track->meta->findInt64(kKeyDuration, &duration)) {
                    track->meta->setInt64(
                            kKeyThumbnailTime, duration / 4);
 }
 } else {
 uint32_t sampleIndex;
 uint32_t sampleTime;
 if (track->timescale != 0 &&
                        track->sampleTable->findThumbnailSample(&sampleIndex) == OK
 && track->sampleTable->getMetaDataForSample(
                            sampleIndex, NULL  , NULL  ,
 &sampleTime) == OK) {
                    track->meta->setInt64(
                            kKeyThumbnailTime,
 ((int64_t)sampleTime * 1000000) / track->timescale);
 }
 }
 }
 }

 return track->meta;
}
