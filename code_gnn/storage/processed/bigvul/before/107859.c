void BlockedPlugin::HidePlugin() {
  CHECK(plugin_);
  hidden_ = true;
  WebPluginContainer* container = plugin_->container();
  WebElement element = container->element();
  element.setAttribute("style", "display: none;");
  if (element.hasAttribute("width") && element.hasAttribute("height")) {
    std::string width_str("width:[\\s]*");
    width_str += element.getAttribute("width").utf8().data();
    if (EndsWith(width_str, "px", false)) {
      width_str = width_str.substr(0, width_str.length() - 2);
    }
    TrimWhitespace(width_str, TRIM_TRAILING, &width_str);
    width_str += "[\\s]*px";
    WebRegularExpression width_regex(WebString::fromUTF8(width_str.c_str()),
                                     WebKit::WebTextCaseSensitive);
    std::string height_str("height:[\\s]*");
    height_str += element.getAttribute("height").utf8().data();
    if (EndsWith(height_str, "px", false)) {
      height_str = height_str.substr(0, height_str.length() - 2);
    }
    TrimWhitespace(height_str, TRIM_TRAILING, &height_str);
    height_str += "[\\s]*px";
    WebRegularExpression height_regex(WebString::fromUTF8(height_str.c_str()),
                                      WebKit::WebTextCaseSensitive);
    WebNode parent = element;
    while (!parent.parentNode().isNull()) {
      parent = parent.parentNode();
      if (!parent.isElementNode())
        continue;
      element = parent.toConst<WebElement>();
      if (element.hasAttribute("style")) {
        WebString style_str = element.getAttribute("style");
        if (width_regex.match(style_str) >= 0 &&
            height_regex.match(style_str) >= 0)
          element.setAttribute("style", "display: none;");
      }
    }
  }
}
