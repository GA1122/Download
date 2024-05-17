DEFINE_TRACE(BluetoothRemoteGATTCharacteristic) {
  visitor->trace(m_service);
  visitor->trace(m_properties);
  visitor->trace(m_value);
  visitor->trace(m_device);
  EventTargetWithInlineData::trace(visitor);
  ContextLifecycleObserver::trace(visitor);
}
