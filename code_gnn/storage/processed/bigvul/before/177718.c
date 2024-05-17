int equalizer_set_band_level(equalizer_context_t *context, int32_t band,
 int32_t level)
{
    ALOGV("%s: band: %d, level: %d", __func__, band, level);
 if (level > 0) {
        level = (int)((level+50)/100);
 } else {
        level = (int)((level-50)/100);
 }
    context->band_levels[band] = level;
    context->preset = PRESET_CUSTOM;

    offload_eq_set_preset(&(context->offload_eq), PRESET_CUSTOM);
    offload_eq_set_bands_level(&(context->offload_eq),
                               NUM_EQ_BANDS,
                               equalizer_band_presets_freq,
                               context->band_levels);
 if (context->ctl)
        offload_eq_send_params(context->ctl, &context->offload_eq,
                               OFFLOAD_SEND_EQ_ENABLE_FLAG |
                               OFFLOAD_SEND_EQ_BANDS_LEVEL);
 return 0;
}
