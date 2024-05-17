std::unique_ptr<TracedValue> FrameLoader::ToTracedValue() const {
  std::unique_ptr<TracedValue> traced_value = TracedValue::Create();
  traced_value->BeginDictionary("frame");
  traced_value->SetString(
      "id_ref", String::Format("0x%" PRIx64,
                               static_cast<uint64_t>(
                                   reinterpret_cast<uintptr_t>(frame_.Get()))));
  traced_value->EndDictionary();
  traced_value->SetBoolean("isLoadingMainFrame", IsLoadingMainFrame());
  traced_value->SetString("stateMachine", state_machine_.ToString());
  traced_value->SetString("provisionalDocumentLoaderURL",
                          provisional_document_loader_
                              ? provisional_document_loader_->Url().GetString()
                              : String());
  traced_value->SetString(
      "documentLoaderURL",
      document_loader_ ? document_loader_->Url().GetString() : String());
  return traced_value;
}
