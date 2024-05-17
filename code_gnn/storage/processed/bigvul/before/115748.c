cricket::SessionDescription* JingleSessionManager::CreateHostSessionDescription(
    const CandidateSessionConfig* config,
    const std::string& certificate) {
  cricket::SessionDescription* desc = new cricket::SessionDescription();
  desc->AddContent(
      ContentDescription::kChromotingContentName, kChromotingXmlNamespace,
      new ContentDescription(config, "", certificate));
  return desc;
}
