int equalizer_set_preset(equalizer_context_t *context, int preset)
{
 int i;

    ALOGV("%s: preset: %d", __func__, preset);
    context->preset = preset;
 for (i=0; i<NUM_EQ_BANDS; i++)
        context->band_levels[i] =
                 equalizer_band_presets_level[i + preset * NUM_EQ_BANDS];

    offload_eq_set_preset(&(context->offload_eq), preset);
    offload_eq_set_bands_level(&(context->offload_eq),
                               NUM_EQ_BANDS,
                               equalizer_band_presets_freq,
                               context->band_levels);
 if(context->ctl)
        offload_eq_send_params(context->ctl, &context->offload_eq,
                               OFFLOAD_SEND_EQ_ENABLE_FLAG |
                               OFFLOAD_SEND_EQ_PRESET);
 return 0;
}
