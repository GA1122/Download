    ChromeContentBrowserClient::GetCryptoPasswordDelegate(
        const GURL& url) {
  return chrome::NewCryptoModuleBlockingDialogDelegate(
      chrome::kCryptoModulePasswordKeygen, url.host());
}
