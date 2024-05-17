FrameFetchContext::FrameFetchContext(DocumentLoader* loader, Document* document)
    : document_loader_(loader),
      document_(document),
      save_data_enabled_(GetNetworkStateNotifier().SaveDataEnabled() &&
                         !GetSettings()->GetDataSaverHoldbackWebApi()) {
  if (document_) {
    fetch_client_settings_object_ =
        new FetchClientSettingsObjectImpl(*document_);
  }
  DCHECK(GetFrame());
}
