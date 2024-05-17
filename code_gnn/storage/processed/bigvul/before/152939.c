    Read(media::mojom::VideoCaptureDeviceDescriptorDataView data,
         media::VideoCaptureDeviceDescriptor* output) {
  std::string display_name;
  if (!data.ReadDisplayName(&display_name))
    return false;
  output->set_display_name(display_name);
  if (!data.ReadDeviceId(&(output->device_id)))
    return false;
  if (!data.ReadModelId(&(output->model_id)))
    return false;
  if (!data.ReadFacingMode(&(output->facing)))
    return false;
  if (!data.ReadCaptureApi(&(output->capture_api)))
    return false;
  if (!data.ReadTransportType(&(output->transport_type)))
    return false;
  if (!data.ReadCameraCalibration(&(output->camera_calibration)))
    return false;
  return true;
}
