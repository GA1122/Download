void WebPluginImpl::ShowModalHTMLDialog(const GURL& url, int width, int height,
                                        const std::string& json_arguments,
                                        std::string* json_retval) {
  if (page_delegate_) {
    page_delegate_->ShowModalHTMLDialogForPlugin(
        url, gfx::Size(width, height), json_arguments, json_retval);
  }
}
