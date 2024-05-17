WebGLRenderingContextBase::GetUnpackPixelStoreParams(TexImageDimension) {
  WebGLImageConversion::PixelStoreParams params;
  params.alignment = unpack_alignment_;
  return params;
}
