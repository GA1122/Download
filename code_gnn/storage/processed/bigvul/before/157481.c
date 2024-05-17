bool IsElevationNeededForSettingDefaultProtocolClient() {
  return base::win::GetVersion() < base::win::VERSION_WIN8;
 }
