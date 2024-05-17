void SpeechRecognitionManagerImpl::SessionStart(const Session& session) {
  DCHECK_EQ(primary_session_id_, session.id);
  const MediaStreamDevices& devices = session.context.devices;
  std::string device_id;
  if (devices.empty()) {
    device_id = media::AudioDeviceDescription::kDefaultDeviceId;
  } else {
    DCHECK_EQ(1u, devices.size());
    DCHECK_EQ(MEDIA_DEVICE_AUDIO_CAPTURE, devices.front().type);
    device_id = devices.front().id;
  }

  session.recognizer->StartRecognition(device_id);
}
