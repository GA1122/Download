 void BluetoothAdapterChromeOS::AddObserver(
    BluetoothAdapter::Observer* observer) {
  DCHECK(observer);
  observers_.AddObserver(observer);
}
