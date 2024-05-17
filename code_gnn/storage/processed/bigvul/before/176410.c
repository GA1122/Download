static void btif_report_source_codec_state(UNUSED_ATTR void* p_data) {
 btav_a2dp_codec_config_t codec_config;
  std::vector<btav_a2dp_codec_config_t> codecs_local_capabilities;
  std::vector<btav_a2dp_codec_config_t> codecs_selectable_capabilities;

  A2dpCodecs* a2dp_codecs = bta_av_get_a2dp_codecs();
 if (a2dp_codecs == nullptr) return;
 if (!a2dp_codecs->getCodecConfigAndCapabilities(
 &codec_config, &codecs_local_capabilities,
 &codecs_selectable_capabilities)) {
    BTIF_TRACE_WARNING(
 "%s: error reporting audio source codec state: "
 "cannot get codec config and capabilities",
        __func__);
 return;
 }
 if (bt_av_src_callbacks != NULL) {
    HAL_CBACK(bt_av_src_callbacks, audio_config_cb, codec_config,
              codecs_local_capabilities, codecs_selectable_capabilities);
 }
}
