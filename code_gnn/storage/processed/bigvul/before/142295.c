ukm::SourceId ChromePasswordManagerClient::GetUkmSourceId() {
  return ukm::GetSourceIdForWebContentsDocument(web_contents());
}
