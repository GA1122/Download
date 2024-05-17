    Read(media::mojom::VideoCaptureDeviceInfoDataView data,
         media::VideoCaptureDeviceInfo* output) {
  if (!data.ReadDescriptor(&(output->descriptor)))
    return false;
  if (!data.ReadSupportedFormats(&(output->supported_formats)))
    return false;
  return true;
}
