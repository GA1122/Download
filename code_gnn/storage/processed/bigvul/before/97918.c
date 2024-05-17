static bool IsWhitelistedForContentSettings(WebFrame* frame) {
  WebSecurityOrigin origin = frame->securityOrigin();
  if (origin.isEmpty())
    return false;   

  if (EqualsASCII(origin.protocol(), chrome::kChromeUIScheme))
    return true;   

  GURL frame_url = frame->url();
  const char* kDirProtocols[] = { "ftp", "file" };
  for (size_t i = 0; i < arraysize(kDirProtocols); ++i) {
    if (EqualsASCII(origin.protocol(), kDirProtocols[i])) {
      return frame_url.SchemeIs(kDirProtocols[i]) &&
             frame_url.ExtractFileName().empty();
    }
  }

  return false;
}