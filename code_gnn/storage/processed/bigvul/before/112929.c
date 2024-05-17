GURL GetFileResourceUrl(const std::string& resource_id,
                        const std::string& file_name) {
  std::string url(base::StringPrintf(
      "%s:%s",
      chrome::kDriveScheme,
      net::EscapePath(resource_id).c_str()));
  return GURL(url);
}
