void TestWebKitPlatformSupport::createIDBKeysFromSerializedValuesAndKeyPath(
      const WebKit::WebVector<WebKit::WebSerializedScriptValue>& values,
      const WebKit::WebString& keyPath,
      WebKit::WebVector<WebKit::WebIDBKey>& keys_out) {
  WebKit::WebVector<WebKit::WebIDBKey> keys(values.size());
  for (size_t i = 0; i < values.size(); ++i) {
    keys[i] = WebKit::WebIDBKey::createFromValueAndKeyPath(
        values[i], WebKit::WebIDBKeyPath::create(keyPath));
  }
  keys_out.swap(keys);
}
