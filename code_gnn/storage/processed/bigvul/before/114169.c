base::StringPiece ChromeContentClient::GetDataResource(int resource_id) const {
  return ResourceBundle::GetSharedInstance().GetRawDataResource(resource_id);
}
