bool GetInnerWebContentsHelper(
    std::vector<WebContentsImpl*>* all_guest_contents,
    WebContents* guest_contents) {
  all_guest_contents->push_back(static_cast<WebContentsImpl*>(guest_contents));
  return false;
}
