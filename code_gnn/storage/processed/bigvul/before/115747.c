JingleSessionManager::CreateClientSessionDescription(
    const CandidateSessionConfig* config,
    const std::string& auth_token) {
  cricket::SessionDescription* desc = new cricket::SessionDescription();
  desc->AddContent(
      ContentDescription::kChromotingContentName, kChromotingXmlNamespace,
      new ContentDescription(config, auth_token, ""));
  return desc;
}
