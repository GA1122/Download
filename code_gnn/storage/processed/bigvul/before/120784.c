void BluetoothAdapterChromeOS::PoweredChanged(bool powered) {
  FOR_EACH_OBSERVER(BluetoothAdapter::Observer, observers_,
                    AdapterPoweredChanged(this, powered));
}
