void SetContentStateIfEmpty(NavigationEntry* entry) {
  if (entry->content_state().empty()) {
    entry->set_content_state(
        webkit_glue::CreateHistoryStateForURL(entry->url()));
  }
}
