    Read(::blink::mojom::WebBluetoothDeviceIdDataView data,
         WTF::String* output) {
  return data.ReadDeviceId(output);
}
