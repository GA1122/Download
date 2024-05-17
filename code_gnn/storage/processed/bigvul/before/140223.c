void BluetoothRemoteGATTCharacteristic::dispatchCharacteristicValueChanged(
    const Vector<uint8_t>& value) {
  this->setValue(BluetoothRemoteGATTUtils::ConvertWTFVectorToDataView(value));
  dispatchEvent(Event::create(EventTypeNames::characteristicvaluechanged));
}
