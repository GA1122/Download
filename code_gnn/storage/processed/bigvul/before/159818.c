bool XSSAuditor::IsLikelySafeResource(const String& url) {
  if (url.IsEmpty() || url == BlankURL().GetString())
    return true;

  if (document_url_.Host().IsEmpty())
    return false;

  KURL resource_url(document_url_, url);
  return (document_url_.Host() == resource_url.Host() &&
          resource_url.Query().IsEmpty());
}
