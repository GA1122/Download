bool CastStreamingNativeHandler::FrameReceiverConfigFromArg(
    v8::Isolate* isolate,
    const v8::Local<v8::Value>& arg,
    media::cast::FrameReceiverConfig* config) const {
  scoped_ptr<V8ValueConverter> converter(V8ValueConverter::create());
  scoped_ptr<base::Value> params_value(
      converter->FromV8Value(arg, context()->v8_context()));
  if (!params_value) {
    isolate->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(isolate, kUnableToConvertParams)));
    return false;
  }
  scoped_ptr<RtpReceiverParams> params =
      RtpReceiverParams::FromValue(*params_value);
  if (!params) {
    isolate->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(isolate, kInvalidRtpParams)));
    return false;
  }

  config->receiver_ssrc = params->receiver_ssrc;
  config->sender_ssrc = params->sender_ssrc;
  config->rtp_max_delay_ms = params->max_latency;
  if (config->rtp_max_delay_ms < 0 || config->rtp_max_delay_ms > 1000) {
    isolate->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(isolate, kInvalidLatency)));
    return false;
  }
  config->channels = 2;
  if (params->codec_name == "OPUS") {
    config->codec = media::cast::CODEC_AUDIO_OPUS;
    config->rtp_timebase = 48000;
    config->rtp_payload_type = media::cast::kDefaultRtpAudioPayloadType;
  } else if (params->codec_name == "PCM16") {
    config->codec = media::cast::CODEC_AUDIO_PCM16;
    config->rtp_timebase = 48000;
    config->rtp_payload_type = media::cast::kDefaultRtpAudioPayloadType;
  } else if (params->codec_name == "AAC") {
    config->codec = media::cast::CODEC_AUDIO_AAC;
    config->rtp_timebase = 48000;
    config->rtp_payload_type = media::cast::kDefaultRtpAudioPayloadType;
  } else if (params->codec_name == "VP8") {
    config->codec = media::cast::CODEC_VIDEO_VP8;
    config->rtp_timebase = 90000;
    config->rtp_payload_type = media::cast::kDefaultRtpVideoPayloadType;
  } else if (params->codec_name == "H264") {
    config->codec = media::cast::CODEC_VIDEO_H264;
    config->rtp_timebase = 90000;
    config->rtp_payload_type = media::cast::kDefaultRtpVideoPayloadType;
  }
  if (params->rtp_timebase) {
    config->rtp_timebase = *params->rtp_timebase;
    if (config->rtp_timebase < 1000 || config->rtp_timebase > 1000000) {
      isolate->ThrowException(v8::Exception::TypeError(
          v8::String::NewFromUtf8(isolate, kInvalidRtpTimebase)));
      return false;
    }
  }
  if (params->aes_key &&
      !HexDecode(*params->aes_key, &config->aes_key)) {
    isolate->ThrowException(v8::Exception::Error(
        v8::String::NewFromUtf8(isolate, kInvalidAesKey)));
    return false;
  }
  if (params->aes_iv_mask &&
      !HexDecode(*params->aes_iv_mask, &config->aes_iv_mask)) {
    isolate->ThrowException(v8::Exception::Error(
        v8::String::NewFromUtf8(isolate, kInvalidAesIvMask)));
    return false;
  }
  return true;
}
