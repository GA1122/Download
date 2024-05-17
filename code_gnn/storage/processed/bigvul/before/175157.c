sp<AMessage> NuPlayer::GenericSource::getTrackInfo(size_t trackIndex) const {
 size_t trackCount = mSources.size();
 if (trackIndex >= trackCount) {
 return NULL;
 }

    sp<AMessage> format = new AMessage();
    sp<MetaData> meta = mSources.itemAt(trackIndex)->getFormat();

 const char *mime;
    CHECK(meta->findCString(kKeyMIMEType, &mime));

 int32_t trackType;
 if (!strncasecmp(mime, "video/", 6)) {
        trackType = MEDIA_TRACK_TYPE_VIDEO;
 } else if (!strncasecmp(mime, "audio/", 6)) {
        trackType = MEDIA_TRACK_TYPE_AUDIO;
 } else if (!strcasecmp(mime, MEDIA_MIMETYPE_TEXT_3GPP)) {
        trackType = MEDIA_TRACK_TYPE_TIMEDTEXT;
 } else {
        trackType = MEDIA_TRACK_TYPE_UNKNOWN;
 }
    format->setInt32("type", trackType);

 const char *lang;
 if (!meta->findCString(kKeyMediaLanguage, &lang)) {
        lang = "und";
 }
    format->setString("language", lang);

 if (trackType == MEDIA_TRACK_TYPE_SUBTITLE) {
        format->setString("mime", mime);

 int32_t isAutoselect = 1, isDefault = 0, isForced = 0;
        meta->findInt32(kKeyTrackIsAutoselect, &isAutoselect);
        meta->findInt32(kKeyTrackIsDefault, &isDefault);
        meta->findInt32(kKeyTrackIsForced, &isForced);

        format->setInt32("auto", !!isAutoselect);
        format->setInt32("default", !!isDefault);
        format->setInt32("forced", !!isForced);
 }

 return format;
}
