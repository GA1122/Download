bool DebuggerFunction::InitClientHost() {
  if (!InitTabContents())
    return false;

  client_host_ = AttachedClientHosts::GetInstance()->Lookup(contents_);

  if (!client_host_ ||
      !client_host_->MatchesContentsAndExtensionId(contents_,
                                                   GetExtension()->id())) {
    error_ = ExtensionErrorUtils::FormatErrorMessage(
        keys::kNotAttachedError,
        base::IntToString(tab_id_));
    return false;
  }
  return true;
}
