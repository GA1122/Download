void AudioRendererHost::OverrideDevicePermissionsForTesting(bool has_access) {
  authorization_handler_.OverridePermissionsForTesting(has_access);
}
