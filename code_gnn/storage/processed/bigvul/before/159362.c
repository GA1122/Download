bool ChromeExtensionsAPIClient::ShouldHideResponseHeader(
    const GURL& url,
    const std::string& header_name) const {
  return (
      (url.host_piece() == GaiaUrls::GetInstance()->gaia_url().host_piece()) &&
      (base::CompareCaseInsensitiveASCII(header_name,
                                          signin::kDiceResponseHeader) == 0));
 }
