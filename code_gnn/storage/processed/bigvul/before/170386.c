status_t MPEG4Extractor::verifyTrack(Track *track) {
 const char *mime;
    CHECK(track->meta->findCString(kKeyMIMEType, &mime));

 uint32_t type;
 const void *data;
 size_t size;
 if (!strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_AVC)) {
 if (!track->meta->findData(kKeyAVCC, &type, &data, &size)
 || type != kTypeAVCC) {
 return ERROR_MALFORMED;
 }
 } else if (!strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_HEVC)) {
 if (!track->meta->findData(kKeyHVCC, &type, &data, &size)
 || type != kTypeHVCC) {
 return ERROR_MALFORMED;
 }
 } else if (!strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_MPEG4)
 || !strcasecmp(mime, MEDIA_MIMETYPE_AUDIO_AAC)) {
 if (!track->meta->findData(kKeyESDS, &type, &data, &size)
 || type != kTypeESDS) {
 return ERROR_MALFORMED;
 }
 }

 if (track->sampleTable == NULL || !track->sampleTable->isValid()) {
        ALOGE("stbl atom missing/invalid.");
 return ERROR_MALFORMED;
 }

 return OK;
}
