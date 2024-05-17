void OSExchangeDataProviderAura::SetURL(const GURL& url,
                                        const base::string16& title) {
  url_ = url;
  title_ = title;
  formats_ |= OSExchangeData::URL;

  SetString(base::UTF8ToUTF16(url.spec()));
}
