void Browser::ViewSource(TabContentsWrapper* contents) {
  DCHECK(contents);

  NavigationEntry* active_entry = contents->controller().GetActiveEntry();
  if (!active_entry)
    return;

  ViewSource(contents, active_entry->url(), active_entry->content_state());
}
