void BluetoothDeviceChooserController::SetTestScanDurationForTesting(
    TestScanDurationSetting setting) {
  switch (setting) {
    case TestScanDurationSetting::IMMEDIATE_TIMEOUT:
      scan_duration_ = 0;
      break;
    case TestScanDurationSetting::NEVER_TIMEOUT:
      scan_duration_ = base::TimeDelta::Max().InSeconds();
      break;
  }
}
