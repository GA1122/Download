bool ClassicPendingScript::CheckMIMETypeBeforeRunScript(
    Document* context_document) const {
  if (!is_external_)
    return true;

  return AllowedByNosniff::MimeTypeAsScript(context_document,
                                            GetResource()->GetResponse());
}
