Response PageHandler::Reload(Maybe<bool> bypassCache,
                             Maybe<std::string> script_to_evaluate_on_load) {
  WebContentsImpl* web_contents = GetWebContents();
  if (!web_contents)
    return Response::InternalError();
  if (web_contents->IsCrashed() ||
      web_contents->GetURL().scheme() == url::kDataScheme ||
      (web_contents->GetController().GetVisibleEntry() &&
       web_contents->GetController().GetVisibleEntry()->IsViewSourceMode())) {
    web_contents->GetController().Reload(bypassCache.fromMaybe(false)
                                             ? ReloadType::BYPASSING_CACHE
                                             : ReloadType::NORMAL,
                                         false);
    return Response::OK();
  } else {
    return Response::FallThrough();
  }
}
