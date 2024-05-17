bool GLES2Implementation::IsExtensionAvailableHelper(const char* extension,
                                                     ExtensionStatus* status) {
  switch (*status) {
    case kAvailableExtensionStatus:
      return true;
    case kUnavailableExtensionStatus:
      return false;
    default: {
      bool available = IsExtensionAvailable(extension);
      *status =
          available ? kAvailableExtensionStatus : kUnavailableExtensionStatus;
      return available;
    }
  }
}
