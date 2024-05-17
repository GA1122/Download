bool OSExchangeDataProviderAuraX11::GetPlainTextURL(GURL* url) const {
  base::string16 text;
  if (GetString(&text)) {
    GURL test_url(text);
    if (test_url.is_valid()) {
      *url = test_url;
      return true;
    }
  }

  return false;
}
