void Browser::RegisterProtocolHandler(TabContents* tab,
                                      const std::string& protocol,
                                      const GURL& url,
                                      const string16& title) {
  RegisterProtocolHandlerHelper(tab, protocol, url, title);
}
