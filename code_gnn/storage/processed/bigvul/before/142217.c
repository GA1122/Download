  RemovableTestVolume(const std::string& name,
                      VolumeType volume_type,
                      chromeos::DeviceType device_type,
                      const base::FilePath& device_path,
                      const std::string& drive_label)
      : LocalTestVolume(name),
        volume_type_(volume_type),
        device_type_(device_type),
        device_path_(device_path),
        drive_label_(drive_label) {}
