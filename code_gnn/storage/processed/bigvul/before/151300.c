std::unique_ptr<TracedValue> InspectorResourceFinishEvent::Data(
    unsigned long identifier,
    double finish_time,
    bool did_fail,
    int64_t encoded_data_length,
    int64_t decoded_body_length) {
  String request_id = IdentifiersFactory::RequestId(identifier);

  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("requestId", request_id);
  value->SetBoolean("didFail", did_fail);
  value->SetDouble("encodedDataLength", encoded_data_length);
  value->SetDouble("decodedBodyLength", decoded_body_length);
  if (finish_time)
    value->SetDouble("finishTime", finish_time);
  return value;
}
