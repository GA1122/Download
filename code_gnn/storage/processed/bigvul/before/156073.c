std::string GetGoogleCountryCode(const GURL& google_homepage_url) {
  base::StringPiece google_hostname = google_homepage_url.host_piece();
  const size_t last_dot = google_hostname.find_last_of('.');
  if (last_dot == std::string::npos)
    return std::string();
  base::StringPiece country_code = google_hostname.substr(last_dot + 1);
  if (country_code == "com")
    return "us";
  if (country_code == "uk")
    return "gb";
  if (country_code == "cat")
    return "es";
  return country_code.as_string();
}
