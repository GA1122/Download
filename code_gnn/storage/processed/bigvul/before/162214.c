void RunEmptyFormatsCallback(
    mojom::VideoCaptureHost::GetDeviceSupportedFormatsCallback& callback) {
  media::VideoCaptureFormats formats;
  std::move(callback).Run(formats);
}
