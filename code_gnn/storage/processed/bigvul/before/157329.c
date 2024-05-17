void SetSinkIdOnMediaThread(scoped_refptr<WebAudioSourceProviderImpl> sink,
                            const std::string& device_id,
                            const OutputDeviceStatusCB& callback) {
  sink->SwitchOutputDevice(device_id, callback);
}
