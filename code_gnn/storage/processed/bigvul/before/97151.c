static bool TreatAsAttachment(const ResourceResponse& response) {
  const String& content_disposition =
      response.httpHeaderField("Content-Disposition");
  if (content_disposition.isEmpty())
    return false;

  if (content_disposition.startsWith(";"))
    return false;

  if (content_disposition.startsWith("inline", false))
    return false;

  if (content_disposition.startsWith("filename", false))
    return false;

  if (content_disposition.startsWith("name", false))
    return false;

  return true;
}
