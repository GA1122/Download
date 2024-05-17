std::unique_ptr<WebContents> WebContents::Create(
    const WebContents::CreateParams& params) {
  return WebContentsImpl::CreateWithOpener(params, FindOpenerRFH(params));
}
