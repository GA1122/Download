void VideoCaptureImpl::OnDeviceSupportedFormats(
    const VideoCaptureDeviceFormatsCB& callback,
    const media::VideoCaptureFormats& supported_formats) {
  DCHECK(io_thread_checker_.CalledOnValidThread());
  callback.Run(supported_formats);
}
