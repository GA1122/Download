int equalizer_get_band_level(equalizer_context_t *context, int32_t band)
{
    ALOGV("%s: band: %d level: %d", __func__, band,
           context->band_levels[band] * 100);
 return context->band_levels[band] * 100;
}
