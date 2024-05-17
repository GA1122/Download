bool SniffAMR(
 const sp<DataSource> &source, String8 *mimeType, float *confidence,
        sp<AMessage> *) {
 char header[9];

 if (source->readAt(0, header, sizeof(header)) != sizeof(header)) {
 return false;
 }

 if (!memcmp(header, "#!AMR\n", 6)) {
 *mimeType = MEDIA_MIMETYPE_AUDIO_AMR_NB;
 *confidence = 0.5;

 return true;
 } else if (!memcmp(header, "#!AMR-WB\n", 9)) {
 *mimeType = MEDIA_MIMETYPE_AUDIO_AMR_WB;
 *confidence = 0.5;

 return true;
 }

 return false;
}
