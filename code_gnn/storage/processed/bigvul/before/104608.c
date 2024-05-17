GURL Extension::GetHomepageURL() const {
  if (homepage_url_.is_valid())
    return homepage_url_;

  if (!UpdatesFromGallery())
    return GURL();

  GURL url(ChromeStoreLaunchURL() + std::string("/detail/") + id());
  return url;
}
