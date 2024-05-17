content::WebContents* DevToolsWindow::GetInTabWebContents(
    WebContents* inspected_web_contents,
    DevToolsContentsResizingStrategy* out_strategy) {
  DevToolsWindow* window = GetInstanceForInspectedWebContents(
      inspected_web_contents);
  if (!window || window->life_stage_ == kClosing)
    return NULL;

  bool is_docked_set = window->life_stage_ == kLoadCompleted ||
      window->life_stage_ == kIsDockedSet;
  if (!is_docked_set)
    return NULL;

  if (!window->is_docked_ && !window->toolbox_web_contents_)
    return NULL;

  if (out_strategy)
    out_strategy->CopyFrom(window->contents_resizing_strategy_);

  return window->is_docked_ ? window->main_web_contents_ :
      window->toolbox_web_contents_;
}
