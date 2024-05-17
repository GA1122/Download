void BluetoothRemoteGATTServer::AddToActiveAlgorithms(
    ScriptPromiseResolver* resolver) {
  auto result = m_activeAlgorithms.add(resolver);
  CHECK(result.isNewEntry);
}
