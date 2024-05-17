static void btif_update_source_codec(void* p_data) {
  BTIF_TRACE_DEBUG("%s", __func__);

 btav_a2dp_codec_config_t req;
  memcpy(&req, p_data, sizeof(req));

  btif_a2dp_source_encoder_user_config_update_req(req);
}
