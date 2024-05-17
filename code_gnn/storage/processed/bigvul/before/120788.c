void BluetoothAdapterChromeOS::RemoveObserver(
    BluetoothAdapter::Observer* observer) {
  DCHECK(observer);
  observers_.RemoveObserver(observer);
}
