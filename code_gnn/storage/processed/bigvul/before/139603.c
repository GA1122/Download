MimeUtil::MimeUtil() : allow_proprietary_codecs_(false) {
#if defined(OS_ANDROID)
  platform_info_.has_platform_decoders = HasPlatformDecoderSupport();
  platform_info_.has_platform_vp8_decoder =
      MediaCodecUtil::IsVp8DecoderAvailable();
  platform_info_.has_platform_vp9_decoder =
      MediaCodecUtil::IsVp9DecoderAvailable();
  platform_info_.supports_opus = PlatformHasOpusSupport();
#endif

  InitializeMimeTypeMaps();
}
