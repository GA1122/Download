    FromMojom(media::mojom::VideoCaptureTransportType input,
              media::VideoCaptureTransportType* output) {
  switch (input) {
    case media::mojom::VideoCaptureTransportType::MACOSX_USB_OR_BUILT_IN:
      *output = media::VideoCaptureTransportType::MACOSX_USB_OR_BUILT_IN;
      return true;
    case media::mojom::VideoCaptureTransportType::OTHER_TRANSPORT:
      *output = media::VideoCaptureTransportType::OTHER_TRANSPORT;
      return true;
  }
  NOTREACHED();
  return false;
}
