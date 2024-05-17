bool ToCastRtpParamsOrThrow(v8::Isolate* isolate,
                            const RtpParams& ext_params,
                            CastRtpParams* cast_params) {
  std::copy(ext_params.rtcp_features.begin(),
            ext_params.rtcp_features.end(),
            std::back_inserter(cast_params->rtcp_features));
  if (!ToCastRtpPayloadParamsOrThrow(isolate,
                                     ext_params.payload,
                                     &cast_params->payload)) {
    return false;
  }
  return true;
}
