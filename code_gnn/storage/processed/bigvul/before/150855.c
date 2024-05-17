bool BluetoothAdapter::HasObserver(BluetoothAdapter::Observer* observer) {
  DCHECK(observer);
  return observers_.HasObserver(observer);
}
