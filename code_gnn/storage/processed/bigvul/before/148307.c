bool WebContentsImpl::AddDomainInfoToRapporSample(rappor::Sample* sample) {
  sample->SetStringField("Domain", ::rappor::GetDomainAndRegistrySampleFromGURL(
                                       GetLastCommittedURL()));
  return true;
}
