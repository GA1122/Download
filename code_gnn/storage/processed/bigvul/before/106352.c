void SyncBackendHost::Core::RouteJsMessageReply(
    const std::string& name, const JsArgList& args,
    const JsEventHandler* target) {
  host_->frontend_loop_->PostTask(
      FROM_HERE, NewRunnableMethod(
          this, &Core::RouteJsMessageReplyOnFrontendLoop,
          name, args, target));
}
