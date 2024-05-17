  static bool MapStringToTargetVolume(base::StringPiece value,
                                      TargetVolume* volume) {
    if (value == "local")
      *volume = LOCAL_VOLUME;
    else if (value == "drive")
      *volume = DRIVE_VOLUME;
    else if (value == "crostini")
      *volume = CROSTINI_VOLUME;
    else if (value == "usb")
      *volume = USB_VOLUME;
    else if (value == "android_files")
      *volume = ANDROID_FILES_VOLUME;
    else if (value == "documents_provider")
      *volume = DOCUMENTS_PROVIDER_VOLUME;
    else
      return false;
    return true;
  }
