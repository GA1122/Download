void DevToolsUIBindings::OpenRemotePage(const std::string& browser_id,
                                        const std::string& url) {
  if (!remote_targets_handler_)
    return;
  remote_targets_handler_->Open(browser_id, url);
}
