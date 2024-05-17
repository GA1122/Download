void ToCastCodecSpecificParams(const CodecSpecificParams& ext_params,
                               CastCodecSpecificParams* cast_params) {
  cast_params->key = ext_params.key;
  cast_params->value = ext_params.value;
}
