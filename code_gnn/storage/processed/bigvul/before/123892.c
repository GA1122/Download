void RenderViewImpl::OnImeSetComposition(
    const string16& text,
    const std::vector<WebKit::WebCompositionUnderline>& underlines,
    int selection_start,
    int selection_end) {
  if (pepper_helper_->IsPluginFocused()) {
    pepper_helper_->OnImeSetComposition(text,
                                        underlines,
                                        selection_start,
                                        selection_end);
  } else {
#if defined(OS_WIN)
    if (focused_plugin_id_ >= 0) {
      std::vector<int> clauses;
      std::vector<int> target;
      for (size_t i = 0; i < underlines.size(); ++i) {
        clauses.push_back(underlines[i].startOffset);
        clauses.push_back(underlines[i].endOffset);
        if (underlines[i].thick) {
          target.clear();
          target.push_back(underlines[i].startOffset);
          target.push_back(underlines[i].endOffset);
        }
      }
      std::set<WebPluginDelegateProxy*>::iterator it;
      for (it = plugin_delegates_.begin();
           it != plugin_delegates_.end(); ++it) {
        (*it)->ImeCompositionUpdated(text, clauses, target, selection_end,
                                     focused_plugin_id_);
      }
      return;
    }
#endif
    RenderWidget::OnImeSetComposition(text,
                                      underlines,
                                      selection_start,
                                      selection_end);
  }
}
