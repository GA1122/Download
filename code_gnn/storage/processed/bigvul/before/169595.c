bool ToCastRtpPayloadParamsOrThrow(v8::Isolate* isolate,
                                   const RtpPayloadParams& ext_params,
                                   CastRtpPayloadParams* cast_params) {
  cast_params->payload_type = ext_params.payload_type;
  cast_params->max_latency_ms = ext_params.max_latency;
  cast_params->min_latency_ms =
      ext_params.min_latency ? *ext_params.min_latency : ext_params.max_latency;
  cast_params->animated_latency_ms = ext_params.animated_latency
                                         ? *ext_params.animated_latency
                                         : ext_params.max_latency;
  cast_params->codec_name = ext_params.codec_name;
  cast_params->ssrc = ext_params.ssrc;
  cast_params->feedback_ssrc = ext_params.feedback_ssrc;
  cast_params->clock_rate = ext_params.clock_rate ? *ext_params.clock_rate : 0;
  cast_params->min_bitrate =
      ext_params.min_bitrate ? *ext_params.min_bitrate : 0;
  cast_params->max_bitrate =
      ext_params.max_bitrate ? *ext_params.max_bitrate : 0;
  cast_params->channels = ext_params.channels ? *ext_params.channels : 0;
  cast_params->max_frame_rate =
      ext_params.max_frame_rate ? *ext_params.max_frame_rate : 0.0;
  if (ext_params.aes_key &&
      !HexDecode(*ext_params.aes_key, &cast_params->aes_key)) {
    isolate->ThrowException(v8::Exception::Error(
        v8::String::NewFromUtf8(isolate, kInvalidAesKey)));
    return false;
  }
  if (ext_params.aes_iv_mask &&
      !HexDecode(*ext_params.aes_iv_mask, &cast_params->aes_iv_mask)) {
    isolate->ThrowException(v8::Exception::Error(
        v8::String::NewFromUtf8(isolate, kInvalidAesIvMask)));
    return false;
  }
  for (size_t i = 0; i < ext_params.codec_specific_params.size(); ++i) {
    CastCodecSpecificParams cast_codec_params;
    ToCastCodecSpecificParams(ext_params.codec_specific_params[i],
                              &cast_codec_params);
    cast_params->codec_specific_params.push_back(cast_codec_params);
  }
  return true;
}
