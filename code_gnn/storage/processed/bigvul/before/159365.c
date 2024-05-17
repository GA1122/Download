int GetWebRequestCountFromBackgroundPage(const Extension* extension,
                                         content::BrowserContext* context) {
  return GetCountFromBackgroundPage(extension, context,
                                    "window.webRequestCount");
}
