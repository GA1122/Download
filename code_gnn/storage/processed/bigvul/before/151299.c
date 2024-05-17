std::unique_ptr<TracedValue> InspectorReceiveDataEvent::Data(
    unsigned long identifier,
    LocalFrame* frame,
    int encoded_data_length) {
  String request_id = IdentifiersFactory::RequestId(identifier);

  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("requestId", request_id);
  value->SetString("frame", ToHexString(frame));
  value->SetInteger("encodedDataLength", encoded_data_length);
  return value;
}
