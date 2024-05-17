Response InspectorPageAgent::reload(
    Maybe<bool> optional_bypass_cache,
    Maybe<String> optional_script_to_evaluate_on_load) {
  pending_script_to_evaluate_on_load_once_ =
      optional_script_to_evaluate_on_load.fromMaybe("");
  v8_session_->setSkipAllPauses(true);
  reloading_ = true;
  inspected_frames_->Root()->Reload(optional_bypass_cache.fromMaybe(false)
                                        ? kFrameLoadTypeReloadBypassingCache
                                        : kFrameLoadTypeReload,
                                    ClientRedirectPolicy::kNotClientRedirect);
  return Response::OK();
}
