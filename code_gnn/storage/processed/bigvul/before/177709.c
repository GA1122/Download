int equalizer_get_band(equalizer_context_t *context __unused, uint32_t freq)
{
 int i;

    ALOGV("%s: freq: %d", __func__, freq);
 for (i = 0; i < NUM_EQ_BANDS; i++) {
 if (freq <= equalizer_band_freq_range[i][1]) {
 return i;
 }
 }
 return NUM_EQ_BANDS - 1;
}
