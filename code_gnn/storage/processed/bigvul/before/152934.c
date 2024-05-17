    FromMojom(media::mojom::VideoCaptureApi input,
              media::VideoCaptureApi* output) {
  switch (input) {
    case media::mojom::VideoCaptureApi::LINUX_V4L2_SINGLE_PLANE:
      *output = media::VideoCaptureApi::LINUX_V4L2_SINGLE_PLANE;
      return true;
    case media::mojom::VideoCaptureApi::WIN_MEDIA_FOUNDATION:
      *output = media::VideoCaptureApi::WIN_MEDIA_FOUNDATION;
      return true;
    case media::mojom::VideoCaptureApi::WIN_MEDIA_FOUNDATION_SENSOR:
      *output = media::VideoCaptureApi::WIN_MEDIA_FOUNDATION_SENSOR;
      return true;
    case media::mojom::VideoCaptureApi::WIN_DIRECT_SHOW:
      *output = media::VideoCaptureApi::WIN_DIRECT_SHOW;
      return true;
    case media::mojom::VideoCaptureApi::MACOSX_AVFOUNDATION:
      *output = media::VideoCaptureApi::MACOSX_AVFOUNDATION;
      return true;
    case media::mojom::VideoCaptureApi::MACOSX_DECKLINK:
      *output = media::VideoCaptureApi::MACOSX_DECKLINK;
      return true;
    case media::mojom::VideoCaptureApi::ANDROID_API1:
      *output = media::VideoCaptureApi::ANDROID_API1;
      return true;
    case media::mojom::VideoCaptureApi::ANDROID_API2_LEGACY:
      *output = media::VideoCaptureApi::ANDROID_API2_LEGACY;
      return true;
    case media::mojom::VideoCaptureApi::ANDROID_API2_FULL:
      *output = media::VideoCaptureApi::ANDROID_API2_FULL;
      return true;
    case media::mojom::VideoCaptureApi::ANDROID_API2_LIMITED:
      *output = media::VideoCaptureApi::ANDROID_API2_LIMITED;
      return true;
    case media::mojom::VideoCaptureApi::VIRTUAL_DEVICE:
      *output = media::VideoCaptureApi::VIRTUAL_DEVICE;
      return true;
    case media::mojom::VideoCaptureApi::UNKNOWN:
      *output = media::VideoCaptureApi::UNKNOWN;
      return true;
  }
  NOTREACHED();
  return false;
}
