void BluetoothAdapterChromeOS::DiscoveringChanged(
    bool discovering) {
  FOR_EACH_OBSERVER(BluetoothAdapter::Observer, observers_,
                    AdapterDiscoveringChanged(this, discovering));
}
