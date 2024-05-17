bool OSExchangeDataProviderAura::GetHtml(base::string16* html,
                                         GURL* base_url) const {
  if ((formats_ & OSExchangeData::HTML) == 0)
    return false;
  *html = html_;
  *base_url = base_url_;
  return true;
}
