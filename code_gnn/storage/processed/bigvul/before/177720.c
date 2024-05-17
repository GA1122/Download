int equalizer_set_parameter(effect_context_t *context, effect_param_t *p,
 uint32_t size __unused)
{
 equalizer_context_t *eq_ctxt = (equalizer_context_t *)context;
 int voffset = ((p->psize - 1) / sizeof(int32_t) + 1) * sizeof(int32_t);
 void *value = p->data + voffset;
 int32_t *param_tmp = (int32_t *)p->data;
 int32_t param = *param_tmp++;
 int32_t preset;
 int32_t band;
 int32_t level;
 int i;

    ALOGV("%s", __func__);

    p->status = 0;

 switch (param) {
 case EQ_PARAM_CUR_PRESET:
	ALOGV("EQ_PARAM_CUR_PRESET");
        preset = (int32_t)(*(uint16_t *)value);

 if ((preset >= equalizer_get_num_presets(eq_ctxt)) || (preset < 0)) {
           p->status = -EINVAL;
 break;
 }
        equalizer_set_preset(eq_ctxt, preset);
 break;
 case EQ_PARAM_BAND_LEVEL:
	ALOGV("EQ_PARAM_BAND_LEVEL");
        band = *param_tmp;
        level = (int32_t)(*(int16_t *)value);
 if (band >= NUM_EQ_BANDS) {
           p->status = -EINVAL;
 break;
 }
        equalizer_set_band_level(eq_ctxt, band, level);
 break;
 case EQ_PARAM_PROPERTIES: {
	ALOGV("EQ_PARAM_PROPERTIES");
 int16_t *prop = (int16_t *)value;
 if ((int)prop[0] >= equalizer_get_num_presets(eq_ctxt)) {
            p->status = -EINVAL;
 break;
 }
 if (prop[0] >= 0) {
            equalizer_set_preset(eq_ctxt, (int)prop[0]);
 } else {
 if ((int)prop[1] != NUM_EQ_BANDS) {
                p->status = -EINVAL;
 break;
 }
 for (i = 0; i < NUM_EQ_BANDS; i++) {
               equalizer_set_band_level(eq_ctxt, i, (int)prop[2 + i]);
 }
 }
 } break;
 default:
        p->status = -EINVAL;
 break;
 }

 return 0;
}
