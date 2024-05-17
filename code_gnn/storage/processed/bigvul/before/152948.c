    media::VideoCaptureTransportType>::ToMojom(media::VideoCaptureTransportType
                                                   input) {
  switch (input) {
    case media::VideoCaptureTransportType::MACOSX_USB_OR_BUILT_IN:
      return media::mojom::VideoCaptureTransportType::MACOSX_USB_OR_BUILT_IN;
    case media::VideoCaptureTransportType::OTHER_TRANSPORT:
      return media::mojom::VideoCaptureTransportType::OTHER_TRANSPORT;
  }
  NOTREACHED();
  return media::mojom::VideoCaptureTransportType::OTHER_TRANSPORT;
}
