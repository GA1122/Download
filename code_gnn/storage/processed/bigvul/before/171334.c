static bool formatHasNotablyChanged(
 const sp<MetaData> &from, const sp<MetaData> &to) {
 if (from.get() == NULL && to.get() == NULL) {
 return false;
 }

 if ((from.get() == NULL && to.get() != NULL)
 || (from.get() != NULL && to.get() == NULL)) {
 return true;
 }

 const char *mime_from, *mime_to;
    CHECK(from->findCString(kKeyMIMEType, &mime_from));
    CHECK(to->findCString(kKeyMIMEType, &mime_to));

 if (strcasecmp(mime_from, mime_to)) {
 return true;
 }

 if (!strcasecmp(mime_from, MEDIA_MIMETYPE_VIDEO_RAW)) {
 int32_t colorFormat_from, colorFormat_to;
        CHECK(from->findInt32(kKeyColorFormat, &colorFormat_from));
        CHECK(to->findInt32(kKeyColorFormat, &colorFormat_to));

 if (colorFormat_from != colorFormat_to) {
 return true;
 }

 int32_t width_from, width_to;
        CHECK(from->findInt32(kKeyWidth, &width_from));
        CHECK(to->findInt32(kKeyWidth, &width_to));

 if (width_from != width_to) {
 return true;
 }

 int32_t height_from, height_to;
        CHECK(from->findInt32(kKeyHeight, &height_from));
        CHECK(to->findInt32(kKeyHeight, &height_to));

 if (height_from != height_to) {
 return true;
 }

 int32_t left_from, top_from, right_from, bottom_from;
        CHECK(from->findRect(
                    kKeyCropRect,
 &left_from, &top_from, &right_from, &bottom_from));

 int32_t left_to, top_to, right_to, bottom_to;
        CHECK(to->findRect(
                    kKeyCropRect,
 &left_to, &top_to, &right_to, &bottom_to));

 if (left_to != left_from || top_to != top_from
 || right_to != right_from || bottom_to != bottom_from) {
 return true;
 }
 } else if (!strcasecmp(mime_from, MEDIA_MIMETYPE_AUDIO_RAW)) {
 int32_t numChannels_from, numChannels_to;
        CHECK(from->findInt32(kKeyChannelCount, &numChannels_from));
        CHECK(to->findInt32(kKeyChannelCount, &numChannels_to));

 if (numChannels_from != numChannels_to) {
 return true;
 }

 int32_t sampleRate_from, sampleRate_to;
        CHECK(from->findInt32(kKeySampleRate, &sampleRate_from));
        CHECK(to->findInt32(kKeySampleRate, &sampleRate_to));

 if (sampleRate_from != sampleRate_to) {
 return true;
 }
 }

 return false;
}
