void BlinkTestRunner::DidChangeBatteryStatus(
    const blink::WebBatteryStatus& status) {
  MockBatteryStatusChanged(status);
}
