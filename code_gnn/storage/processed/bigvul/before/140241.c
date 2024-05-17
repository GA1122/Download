ScriptPromise BluetoothRemoteGATTServer::connect(ScriptState* scriptState) {
  m_device->bluetooth()->addDevice(device()->id(), device());

  ScriptPromiseResolver* resolver = ScriptPromiseResolver::create(scriptState);
  ScriptPromise promise = resolver->promise();

  mojom::blink::WebBluetoothService* service = m_device->bluetooth()->service();
  service->RemoteServerConnect(
      device()->id(), convertToBaseCallback(WTF::bind(
                          &BluetoothRemoteGATTServer::ConnectCallback,
                          wrapPersistent(this), wrapPersistent(resolver))));

  return promise;
}
