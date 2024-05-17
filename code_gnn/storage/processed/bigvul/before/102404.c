void GetIdentityFromURL(const GURL& url,
                        string16* username,
                        string16* password) {
  UnescapeRule::Type flags =
      UnescapeRule::SPACES | UnescapeRule::URL_SPECIAL_CHARS;
  *username = UnescapeAndDecodeUTF8URLComponent(url.username(), flags, NULL);
  *password = UnescapeAndDecodeUTF8URLComponent(url.password(), flags, NULL);
}
