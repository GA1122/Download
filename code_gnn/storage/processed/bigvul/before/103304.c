std::string GpuDataManager::GetBlacklistVersion() const {
  if (gpu_blacklist_.get() != NULL) {
    uint16 version_major, version_minor;
    if (gpu_blacklist_->GetVersion(&version_major,
                                   &version_minor)) {
      std::string version_string =
          base::UintToString(static_cast<unsigned>(version_major)) +
          "." +
          base::UintToString(static_cast<unsigned>(version_minor));
      return version_string;
    }
  }
  return "";
}
