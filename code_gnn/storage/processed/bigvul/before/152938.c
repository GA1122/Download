    Read(media::mojom::VideoCaptureDeviceDescriptorCameraCalibrationDataView
             data,
         media::VideoCaptureDeviceDescriptor::CameraCalibration* output) {
  output->focal_length_x = data.focal_length_x();
  output->focal_length_y = data.focal_length_y();
  output->depth_near = data.depth_near();
  output->depth_far = data.depth_far();
  return true;
}
