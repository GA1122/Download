void FromCastRtpParams(const CastRtpParams& cast_params,
                       RtpParams* ext_params) {
  std::copy(cast_params.rtcp_features.begin(),
            cast_params.rtcp_features.end(),
            std::back_inserter(ext_params->rtcp_features));
  FromCastRtpPayloadParams(cast_params.payload, &ext_params->payload);
}
