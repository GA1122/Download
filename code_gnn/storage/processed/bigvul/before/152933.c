    FromMojom(media::mojom::VideoFacingMode input,
              media::VideoFacingMode* output) {
  switch (input) {
    case media::mojom::VideoFacingMode::NONE:
      *output = media::VideoFacingMode::MEDIA_VIDEO_FACING_NONE;
      return true;
    case media::mojom::VideoFacingMode::USER:
      *output = media::VideoFacingMode::MEDIA_VIDEO_FACING_USER;
      return true;
    case media::mojom::VideoFacingMode::ENVIRONMENT:
      *output = media::VideoFacingMode::MEDIA_VIDEO_FACING_ENVIRONMENT;
      return true;
  }
  NOTREACHED();
  return false;
}
