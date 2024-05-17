int equalizer_get_center_frequency(equalizer_context_t *context __unused, int32_t band)
{
    ALOGV("%s: band: %d", __func__, band);
 return (equalizer_band_freq_range[band][0] +
            equalizer_band_freq_range[band][1]) / 2;
}
