void FromCastCodecSpecificParams(const CastCodecSpecificParams& cast_params,
                                 CodecSpecificParams* ext_params) {
  ext_params->key = cast_params.key;
  ext_params->value = cast_params.value;
}
