void FrameFetchContext::SendImagePing(const KURL& url) {
  if (IsDetached())
    return;
  PingLoader::LoadImage(GetFrame(), url);
}
