void PluginInstance::NPP_URLNotify(const char *url,
                                   NPReason reason,
                                   void *notifyData) {
  DCHECK(npp_functions_ != 0);
  DCHECK(npp_functions_->urlnotify != 0);
  if (npp_functions_->urlnotify != 0) {
    npp_functions_->urlnotify(npp_, url, reason, notifyData);
  }
}
