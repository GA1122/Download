void Browser::RegisterProtocolHandler(WebContents* web_contents,
                                      const std::string& protocol,
                                      const GURL& url,
                                      const string16& title,
                                      bool user_gesture) {
  RegisterProtocolHandlerHelper(
      web_contents, protocol, url, title, user_gesture, window());
}
