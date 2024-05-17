  void BluetoothOptionsHandler::GenerateFakeDeviceList() {
  GenerateFakeDiscoveredDevice(
//   GenerateFakeDevice(
// 
      "Fake Wireless Keyboard",
      "01-02-03-04-05-06",
      "input-keyboard",
      true,
    true);
  GenerateFakeDiscoveredDevice(
//     true,
//     "");
//   GenerateFakeDevice(
      "Fake Wireless Mouse",
      "02-03-04-05-06-01",
      "input-mouse",
      true,
    false);
  GenerateFakeDiscoveredDevice(
//     false,
//     "");
//   GenerateFakeDevice(
      "Fake Wireless Headset",
      "03-04-05-06-01-02",
      "headset",
      false,
    false);
  GenerateFakePairing(
//     false,
//     "");
//   GenerateFakeDevice(
      "Fake Connecting Keyboard",
      "04-05-06-01-02-03",
      "input-keyboard",
//     false,
//     false,
      "bluetoothRemotePasskey");
  GenerateFakePairing(
//   GenerateFakeDevice(
      "Fake Connecting Phone",
      "05-06-01-02-03-04",
      "phone",
//     false,
//     false,
      "bluetoothConfirmPasskey");
  GenerateFakePairing(
//   GenerateFakeDevice(
      "Fake Connecting Headset",
      "06-01-02-03-04-05",
      "headset",
//     false,
//     false,
      "bluetoothEnterPasskey");
  
    web_ui_->CallJavascriptFunction(
        "options.SystemOptions.notifyBluetoothSearchComplete");
  }