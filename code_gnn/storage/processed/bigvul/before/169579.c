void FromCastRtpPayloadParams(const CastRtpPayloadParams& cast_params,
                              RtpPayloadParams* ext_params) {
  ext_params->payload_type = cast_params.payload_type;
  ext_params->max_latency = cast_params.max_latency_ms;
  ext_params->min_latency.reset(new int(cast_params.min_latency_ms));
  ext_params->animated_latency.reset(new int(cast_params.animated_latency_ms));
  ext_params->codec_name = cast_params.codec_name;
  ext_params->ssrc = cast_params.ssrc;
  ext_params->feedback_ssrc = cast_params.feedback_ssrc;
  if (cast_params.clock_rate)
    ext_params->clock_rate.reset(new int(cast_params.clock_rate));
  if (cast_params.min_bitrate)
    ext_params->min_bitrate.reset(new int(cast_params.min_bitrate));
  if (cast_params.max_bitrate)
    ext_params->max_bitrate.reset(new int(cast_params.max_bitrate));
  if (cast_params.channels)
    ext_params->channels.reset(new int(cast_params.channels));
  if (cast_params.max_frame_rate > 0.0)
    ext_params->max_frame_rate.reset(new double(cast_params.max_frame_rate));
  for (size_t i = 0; i < cast_params.codec_specific_params.size(); ++i) {
    CodecSpecificParams ext_codec_params;
    FromCastCodecSpecificParams(cast_params.codec_specific_params[i],
                                &ext_codec_params);
    ext_params->codec_specific_params.push_back(std::move(ext_codec_params));
  }
}
