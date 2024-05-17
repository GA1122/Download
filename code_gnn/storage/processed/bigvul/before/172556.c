void SoftAAC2::configureDownmix() const {
 char value[PROPERTY_VALUE_MAX];
 if (!(property_get("media.aac_51_output_enabled", value, NULL)
 && (!strcmp(value, "1") || !strcasecmp(value, "true")))) {
        ALOGI("limiting to stereo output");
        aacDecoder_SetParam(mAACDecoder, AAC_PCM_MAX_OUTPUT_CHANNELS, 2);
 }
}
