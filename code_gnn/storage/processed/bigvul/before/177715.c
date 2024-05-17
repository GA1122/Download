const char * equalizer_get_preset_name(equalizer_context_t *context __unused,
 int32_t preset)
{
    ALOGV("%s: preset: %s", __func__, equalizer_preset_names[preset]);
 if (preset == PRESET_CUSTOM) {
 return "Custom";
 } else {
 return equalizer_preset_names[preset];
 }
}
