bool SniffMPEG4(
 const sp<DataSource> &source, String8 *mimeType, float *confidence,
        sp<AMessage> *meta) {
 if (BetterSniffMPEG4(source, mimeType, confidence, meta)) {
 return true;
 }

 if (LegacySniffMPEG4(source, mimeType, confidence)) {
        ALOGW("Identified supported mpeg4 through LegacySniffMPEG4.");
 return true;
 }

 return false;
}
