std::unique_ptr<TracedValue> FrameLoader::ToTracedValue() const {
  std::unique_ptr<TracedValue> traced_value = TracedValue::Create();
  traced_value->BeginDictionary("frame");
  traced_value->SetString("id_ref", IdentifiersFactory::FrameId(frame_.Get()));
  traced_value->EndDictionary();
  traced_value->SetBoolean("isLoadingMainFrame", frame_->IsMainFrame());
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
