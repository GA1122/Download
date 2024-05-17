bool DXVAVideoDecodeAccelerator::SetDecoderMediaTypes() {
  RETURN_ON_FAILURE(SetDecoderInputMediaType(),
                    "Failed to set decoder input media type", false);
  return SetDecoderOutputMediaType(MFVideoFormat_NV12);
}
