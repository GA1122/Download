std::unique_ptr<content::WebContents> Browser::SwapTabContents(
    content::WebContents* old_contents,
    std::unique_ptr<content::WebContents> new_contents,
    bool did_start_load,
    bool did_finish_load) {
  if (old_contents && new_contents) {
    RenderWidgetHostView* old_view = old_contents->GetMainFrame()->GetView();
    RenderWidgetHostView* new_view = new_contents->GetMainFrame()->GetView();
    if (old_view && new_view)
      new_view->TakeFallbackContentFrom(old_view);
  }

  resource_coordinator::TabLoadTracker::Get()->SwapTabContents(
      old_contents, new_contents.get());

  int index = tab_strip_model_->GetIndexOfWebContents(old_contents);
  DCHECK_NE(TabStripModel::kNoTab, index);
  return tab_strip_model_->ReplaceWebContentsAt(index, std::move(new_contents));
}
