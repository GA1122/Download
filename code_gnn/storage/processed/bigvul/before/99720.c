bool VaapiWrapper::VADisplayState::VAAPIVersionLessThan(int major, int minor) {
  return (major_version_ < major) ||
      (major_version_ == major && minor_version_ < minor);
}
