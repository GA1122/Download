static bt_status_t codec_config_src(
    std::vector<btav_a2dp_codec_config_t> codec_preferences) {
  BTIF_TRACE_EVENT("%s", __func__);
  CHECK_BTAV_INIT();

 for (auto cp : codec_preferences) {
    BTIF_TRACE_DEBUG(
 "%s: codec_type=%d codec_priority=%d "
 "sample_rate=0x%x bits_per_sample=0x%x "
 "channel_mode=0x%x codec_specific_1=%d "
 "codec_specific_2=%d codec_specific_3=%d "
 "codec_specific_4=%d",
        __func__, cp.codec_type, cp.codec_priority, cp.sample_rate,
        cp.bits_per_sample, cp.channel_mode, cp.codec_specific_1,
        cp.codec_specific_2, cp.codec_specific_3, cp.codec_specific_4);
    btif_transfer_context(btif_av_handle_event, BTIF_AV_SOURCE_CONFIG_REQ_EVT,
 reinterpret_cast<char*>(&cp), sizeof(cp), NULL);
 }

 return BT_STATUS_SUCCESS;
}
