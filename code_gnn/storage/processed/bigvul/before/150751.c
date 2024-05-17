int BluetoothDeviceChooserController::CalculateSignalStrengthLevel(
    int8_t rssi) {
  RecordRSSISignalStrength(rssi);

  if (rssi < k20thPercentileRSSI) {
    RecordRSSISignalStrengthLevel(content::UMARSSISignalStrengthLevel::LEVEL_0);
    return 0;
  } else if (rssi < k40thPercentileRSSI) {
    RecordRSSISignalStrengthLevel(content::UMARSSISignalStrengthLevel::LEVEL_1);
    return 1;
  } else if (rssi < k60thPercentileRSSI) {
    RecordRSSISignalStrengthLevel(content::UMARSSISignalStrengthLevel::LEVEL_2);
    return 2;
  } else if (rssi < k80thPercentileRSSI) {
    RecordRSSISignalStrengthLevel(content::UMARSSISignalStrengthLevel::LEVEL_3);
    return 3;
  } else {
    RecordRSSISignalStrengthLevel(content::UMARSSISignalStrengthLevel::LEVEL_4);
    return 4;
  }
}
