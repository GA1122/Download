           media::VideoPixelFormat>::ToMojom(media::VideoPixelFormat input) {
  switch (input) {
    case media::VideoPixelFormat::PIXEL_FORMAT_UNKNOWN:
      return media::mojom::VideoCapturePixelFormat::UNKNOWN;
    case media::VideoPixelFormat::PIXEL_FORMAT_I420:
      return media::mojom::VideoCapturePixelFormat::I420;
    case media::VideoPixelFormat::PIXEL_FORMAT_YV12:
      return media::mojom::VideoCapturePixelFormat::YV12;
    case media::VideoPixelFormat::PIXEL_FORMAT_I422:
      return media::mojom::VideoCapturePixelFormat::I422;
    case media::VideoPixelFormat::PIXEL_FORMAT_I420A:
      return media::mojom::VideoCapturePixelFormat::I420A;
    case media::VideoPixelFormat::PIXEL_FORMAT_I444:
      return media::mojom::VideoCapturePixelFormat::I444;
    case media::VideoPixelFormat::PIXEL_FORMAT_NV12:
      return media::mojom::VideoCapturePixelFormat::NV12;
    case media::VideoPixelFormat::PIXEL_FORMAT_NV21:
      return media::mojom::VideoCapturePixelFormat::NV21;
    case media::VideoPixelFormat::PIXEL_FORMAT_UYVY:
      return media::mojom::VideoCapturePixelFormat::UYVY;
    case media::VideoPixelFormat::PIXEL_FORMAT_YUY2:
      return media::mojom::VideoCapturePixelFormat::YUY2;
    case media::VideoPixelFormat::PIXEL_FORMAT_ARGB:
      return media::mojom::VideoCapturePixelFormat::ARGB;
    case media::VideoPixelFormat::PIXEL_FORMAT_XRGB:
      return media::mojom::VideoCapturePixelFormat::XRGB;
    case media::VideoPixelFormat::PIXEL_FORMAT_RGB24:
      return media::mojom::VideoCapturePixelFormat::RGB24;
    case media::VideoPixelFormat::PIXEL_FORMAT_RGB32:
      return media::mojom::VideoCapturePixelFormat::RGB32;
    case media::VideoPixelFormat::PIXEL_FORMAT_MJPEG:
      return media::mojom::VideoCapturePixelFormat::MJPEG;
    case media::VideoPixelFormat::PIXEL_FORMAT_MT21:
      return media::mojom::VideoCapturePixelFormat::MT21;
    case media::VideoPixelFormat::PIXEL_FORMAT_YUV420P9:
      return media::mojom::VideoCapturePixelFormat::YUV420P9;
    case media::VideoPixelFormat::PIXEL_FORMAT_YUV420P10:
      return media::mojom::VideoCapturePixelFormat::YUV420P10;
    case media::VideoPixelFormat::PIXEL_FORMAT_YUV422P9:
      return media::mojom::VideoCapturePixelFormat::YUV422P9;
    case media::VideoPixelFormat::PIXEL_FORMAT_YUV422P10:
      return media::mojom::VideoCapturePixelFormat::YUV422P10;
    case media::VideoPixelFormat::PIXEL_FORMAT_YUV444P9:
      return media::mojom::VideoCapturePixelFormat::YUV444P9;
    case media::VideoPixelFormat::PIXEL_FORMAT_YUV444P10:
      return media::mojom::VideoCapturePixelFormat::YUV444P10;
    case media::VideoPixelFormat::PIXEL_FORMAT_YUV420P12:
      return media::mojom::VideoCapturePixelFormat::YUV420P12;
    case media::VideoPixelFormat::PIXEL_FORMAT_YUV422P12:
      return media::mojom::VideoCapturePixelFormat::YUV422P12;
    case media::VideoPixelFormat::PIXEL_FORMAT_YUV444P12:
      return media::mojom::VideoCapturePixelFormat::YUV444P12;
    case media::VideoPixelFormat::PIXEL_FORMAT_Y16:
      return media::mojom::VideoCapturePixelFormat::Y16;
  }
  NOTREACHED();
  return media::mojom::VideoCapturePixelFormat::I420;
}
