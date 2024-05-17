void PageHandler::Reload(Maybe<bool> bypassCache,
                         Maybe<std::string> script_to_evaluate_on_load,
                         std::unique_ptr<ReloadCallback> callback) {
  WebContentsImpl* web_contents = GetWebContents();
  if (!web_contents) {
    callback->sendFailure(Response::InternalError());
    return;
  }
  callback->fallThrough();
  web_contents->GetController().Reload(bypassCache.fromMaybe(false)
                                           ? ReloadType::BYPASSING_CACHE
                                           : ReloadType::NORMAL,
                                       false);
}
