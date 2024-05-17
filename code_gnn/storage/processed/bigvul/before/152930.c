    FromMojom(media::mojom::VideoCapturePixelFormat input,
              media::VideoPixelFormat* output) {
  switch (input) {
    case media::mojom::VideoCapturePixelFormat::UNKNOWN:
      *output = media::PIXEL_FORMAT_UNKNOWN;
      return true;
    case media::mojom::VideoCapturePixelFormat::I420:
      *output = media::PIXEL_FORMAT_I420;
      return true;
    case media::mojom::VideoCapturePixelFormat::YV12:
      *output = media::PIXEL_FORMAT_YV12;
      return true;
    case media::mojom::VideoCapturePixelFormat::I422:
      *output = media::PIXEL_FORMAT_I422;
      return true;
    case media::mojom::VideoCapturePixelFormat::I420A:
      *output = media::PIXEL_FORMAT_I420A;
      return true;
    case media::mojom::VideoCapturePixelFormat::I444:
      *output = media::PIXEL_FORMAT_I444;
      return true;
    case media::mojom::VideoCapturePixelFormat::NV12:
      *output = media::PIXEL_FORMAT_NV12;
      return true;
    case media::mojom::VideoCapturePixelFormat::NV21:
      *output = media::PIXEL_FORMAT_NV21;
      return true;
    case media::mojom::VideoCapturePixelFormat::UYVY:
      *output = media::PIXEL_FORMAT_UYVY;
      return true;
    case media::mojom::VideoCapturePixelFormat::YUY2:
      *output = media::PIXEL_FORMAT_YUY2;
      return true;
    case media::mojom::VideoCapturePixelFormat::ARGB:
      *output = media::PIXEL_FORMAT_ARGB;
      return true;
    case media::mojom::VideoCapturePixelFormat::XRGB:
      *output = media::PIXEL_FORMAT_XRGB;
      return true;
    case media::mojom::VideoCapturePixelFormat::RGB24:
      *output = media::PIXEL_FORMAT_RGB24;
      return true;
    case media::mojom::VideoCapturePixelFormat::RGB32:
      *output = media::PIXEL_FORMAT_RGB32;
      return true;
    case media::mojom::VideoCapturePixelFormat::MJPEG:
      *output = media::PIXEL_FORMAT_MJPEG;
      return true;
    case media::mojom::VideoCapturePixelFormat::MT21:
      *output = media::PIXEL_FORMAT_MT21;
      return true;
    case media::mojom::VideoCapturePixelFormat::YUV420P9:
      *output = media::PIXEL_FORMAT_YUV420P9;
      return true;
    case media::mojom::VideoCapturePixelFormat::YUV420P10:
      *output = media::PIXEL_FORMAT_YUV420P10;
      return true;
    case media::mojom::VideoCapturePixelFormat::YUV422P9:
      *output = media::PIXEL_FORMAT_YUV422P9;
      return true;
    case media::mojom::VideoCapturePixelFormat::YUV422P10:
      *output = media::PIXEL_FORMAT_YUV422P10;
      return true;
    case media::mojom::VideoCapturePixelFormat::YUV444P9:
      *output = media::PIXEL_FORMAT_YUV444P9;
      return true;
    case media::mojom::VideoCapturePixelFormat::YUV444P10:
      *output = media::PIXEL_FORMAT_YUV444P10;
      return true;
    case media::mojom::VideoCapturePixelFormat::YUV420P12:
      *output = media::PIXEL_FORMAT_YUV420P12;
      return true;
    case media::mojom::VideoCapturePixelFormat::YUV422P12:
      *output = media::PIXEL_FORMAT_YUV422P12;
      return true;
    case media::mojom::VideoCapturePixelFormat::YUV444P12:
      *output = media::PIXEL_FORMAT_YUV444P12;
      return true;
    case media::mojom::VideoCapturePixelFormat::Y16:
      *output = media::PIXEL_FORMAT_Y16;
      return true;
  }
  NOTREACHED();
  return false;
}
