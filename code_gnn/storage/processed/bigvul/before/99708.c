bool VaapiWrapper::IsJpegDecodeSupported() {
  return profile_infos_.Get().IsProfileSupported(kDecode,
                                                 VAProfileJPEGBaseline);
}
