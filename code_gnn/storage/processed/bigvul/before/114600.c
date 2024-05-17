void RenderThreadImpl::OnTempCrashWithData(const GURL& data) {
  content::GetContentClient()->SetActiveURL(data);
  CHECK(false);
}
