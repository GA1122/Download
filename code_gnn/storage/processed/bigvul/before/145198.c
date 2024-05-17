bool WebstoreBindings::GetWebstoreItemIdFromFrame(
    blink::WebLocalFrame* frame,
    const std::string& preferred_store_link_url,
    std::string* webstore_item_id,
    std::string* error) {
  if (frame != frame->top()) {
    *error = kNotInTopFrameError;
    return false;
  }

  if (!WebUserGestureIndicator::isProcessingUserGesture()) {
    *error = kNotUserGestureError;
    return false;
  }

  WebDocument document = frame->document();
  if (document.isNull()) {
    *error = kNoWebstoreItemLinkFoundError;
    return false;
  }

  WebElement head = document.head();
  if (head.isNull()) {
    *error = kNoWebstoreItemLinkFoundError;
    return false;
  }

  GURL webstore_base_url =
      GURL(extension_urls::GetWebstoreItemDetailURLPrefix());
  for (WebNode child = head.firstChild(); !child.isNull();
       child = child.nextSibling()) {
    if (!child.isElementNode())
      continue;
    WebElement elem = child.to<WebElement>();

    if (!elem.hasHTMLTagName("link") || !elem.hasAttribute("rel") ||
        !elem.hasAttribute("href"))
      continue;

    std::string rel = elem.getAttribute("rel").utf8();
    if (!base::LowerCaseEqualsASCII(rel, kWebstoreLinkRelation))
      continue;

    std::string webstore_url_string(elem.getAttribute("href").utf8());

    if (!preferred_store_link_url.empty() &&
        preferred_store_link_url != webstore_url_string) {
      continue;
    }

    GURL webstore_url = GURL(webstore_url_string);
    if (!webstore_url.is_valid()) {
      *error = kInvalidWebstoreItemUrlError;
      return false;
    }

    if (webstore_url.scheme() != webstore_base_url.scheme() ||
        webstore_url.host() != webstore_base_url.host() ||
        !base::StartsWith(webstore_url.path(), webstore_base_url.path(),
                          base::CompareCase::SENSITIVE)) {
      *error = kInvalidWebstoreItemUrlError;
      return false;
    }

    std::string candidate_webstore_item_id = webstore_url.path().substr(
        webstore_base_url.path().length());
    if (!crx_file::id_util::IdIsValid(candidate_webstore_item_id)) {
      *error = kInvalidWebstoreItemUrlError;
      return false;
    }

    std::string reconstructed_webstore_item_url_string =
        extension_urls::GetWebstoreItemDetailURLPrefix() +
            candidate_webstore_item_id;
    if (reconstructed_webstore_item_url_string != webstore_url_string) {
      *error = kInvalidWebstoreItemUrlError;
      return false;
    }

    *webstore_item_id = candidate_webstore_item_id;
    return true;
  }

  *error = kNoWebstoreItemLinkFoundError;
  return false;
}
