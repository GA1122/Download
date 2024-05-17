void HTMLMediaElement::ResolveScheduledPlayPromises() {
  for (auto& resolver : play_promise_resolve_list_)
    resolver->Resolve();

  play_promise_resolve_list_.clear();
}
