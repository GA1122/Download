Settings* DocumentInit::GetSettings() const {
  DCHECK(MasterDocumentLoader());
  return MasterDocumentLoader()->GetFrame()->GetSettings();
}
