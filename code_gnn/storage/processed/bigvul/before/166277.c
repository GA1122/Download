void VideoCaptureManager::OnDeviceInfosReceived(
    base::ElapsedTimer* timer,
    EnumerationCallback client_callback,
    const std::vector<media::VideoCaptureDeviceInfo>& device_infos) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  UMA_HISTOGRAM_TIMES(
      "Media.VideoCaptureManager.GetAvailableDevicesInfoOnDeviceThreadTime",
      timer->Elapsed());
  devices_info_cache_ = device_infos;

  std::ostringstream string_stream;
  string_stream << "VideoCaptureManager::OnDeviceInfosReceived: Recevied "
                << device_infos.size() << " device infos.";
  for (const auto& entry : device_infos) {
    string_stream << std::endl
                  << "device_id: " << entry.descriptor.device_id
                  << ", display_name: " << entry.descriptor.display_name();
  }
  EmitLogMessage(string_stream.str(), 1);

  media::VideoCaptureDeviceDescriptors devices;
  std::vector<std::tuple<media::VideoCaptureDeviceDescriptor,
                         media::VideoCaptureFormats>>
      descriptors_and_formats;
  for (const auto& it : devices_info_cache_) {
    devices.emplace_back(it.descriptor);
    descriptors_and_formats.emplace_back(it.descriptor, it.supported_formats);
    MediaInternals::GetInstance()->UpdateVideoCaptureDeviceCapabilities(
        descriptors_and_formats);
  }

  std::move(client_callback).Run(devices);
}
