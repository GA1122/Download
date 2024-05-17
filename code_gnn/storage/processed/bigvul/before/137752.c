HeadlessWebContentsImpl* HeadlessWebContentsImpl::From(
    HeadlessWebContents* web_contents) {
  return static_cast<HeadlessWebContentsImpl*>(web_contents);
}
