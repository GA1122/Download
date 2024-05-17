void OMXCodec::clearCodecSpecificData() {
 for (size_t i = 0; i < mCodecSpecificData.size(); ++i) {
        free(mCodecSpecificData.editItemAt(i));
 }
    mCodecSpecificData.clear();
    mCodecSpecificDataIndex = 0;
}
