TestWebKitPlatformSupport::injectIDBKeyIntoSerializedValue(
    const WebKit::WebIDBKey& key,
    const WebKit::WebSerializedScriptValue& value,
    const WebKit::WebString& keyPath) {
  return WebKit::WebIDBKey::injectIDBKeyIntoSerializedValue(
      key, value, WebKit::WebIDBKeyPath::create(keyPath));
}
