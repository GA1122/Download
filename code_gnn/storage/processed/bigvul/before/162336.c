    ToMojom(media::JpegDecodeAccelerator::Error error) {
  switch (error) {
    case media::JpegDecodeAccelerator::NO_ERRORS:
      return media::mojom::DecodeError::NO_ERRORS;
    case media::JpegDecodeAccelerator::INVALID_ARGUMENT:
      return media::mojom::DecodeError::INVALID_ARGUMENT;
    case media::JpegDecodeAccelerator::UNREADABLE_INPUT:
      return media::mojom::DecodeError::UNREADABLE_INPUT;
    case media::JpegDecodeAccelerator::PARSE_JPEG_FAILED:
      return media::mojom::DecodeError::PARSE_JPEG_FAILED;
    case media::JpegDecodeAccelerator::UNSUPPORTED_JPEG:
      return media::mojom::DecodeError::UNSUPPORTED_JPEG;
    case media::JpegDecodeAccelerator::PLATFORM_FAILURE:
      return media::mojom::DecodeError::PLATFORM_FAILURE;
  }
  NOTREACHED();
  return media::mojom::DecodeError::NO_ERRORS;
}
