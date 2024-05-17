bool DownloadUIAdapterDelegate::IsVisibleInUI(const ClientId& client_id) {
  const std::string& name_space = client_id.name_space;
  return model_->GetPolicyController()->IsSupportedByDownload(name_space) &&
         base::IsValidGUID(client_id.id);
}
