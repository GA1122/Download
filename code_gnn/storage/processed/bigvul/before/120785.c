void BluetoothAdapterChromeOS::PresentChanged(bool present) {
  FOR_EACH_OBSERVER(BluetoothAdapter::Observer, observers_,
                    AdapterPresentChanged(this, present));
}
