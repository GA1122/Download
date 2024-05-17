base::string16 ExtensionInstallPrompt::Prompt::GetRetainedDeviceMessageString(
    size_t index) const {
  CHECK_LT(index, retained_device_messages_.size());
  return retained_device_messages_[index];
}
