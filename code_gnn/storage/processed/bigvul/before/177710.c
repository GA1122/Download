int equalizer_get_band_freq_range(equalizer_context_t *context __unused, int32_t band,
 uint32_t *low, uint32_t *high)
{
    ALOGV("%s: band: %d", __func__, band);
 *low = equalizer_band_freq_range[band][0];
 *high = equalizer_band_freq_range[band][1];
 return 0;
}
