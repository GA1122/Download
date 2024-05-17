std::unique_ptr<TracedValue> InspectorReceiveResponseEvent::Data(
    unsigned long identifier,
    LocalFrame* frame,
    const ResourceResponse& response) {
  String request_id = IdentifiersFactory::RequestId(identifier);

  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("requestId", request_id);
  value->SetString("frame", ToHexString(frame));
  value->SetInteger("statusCode", response.HttpStatusCode());
  value->SetString("mimeType", response.MimeType().GetString().IsolatedCopy());
  value->SetDouble("encodedDataLength", response.EncodedDataLength());
  value->SetBoolean("fromCache", response.WasCached());
  value->SetBoolean("fromServiceWorker", response.WasFetchedViaServiceWorker());
  if (response.GetResourceLoadTiming()) {
    value->BeginDictionary("timing");
    RecordTiming(*response.GetResourceLoadTiming(), value.get());
    value->EndDictionary();
  }
  if (response.WasFetchedViaServiceWorker())
    value->SetBoolean("fromServiceWorker", true);
  return value;
}
