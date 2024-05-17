void Browser::SwapTabContents(content::WebContents* old_contents,
                              content::WebContents* new_contents,
                              bool did_start_load,
                              bool did_finish_load) {
  if (old_contents && new_contents) {
    RenderWidgetHostView* old_view = old_contents->GetMainFrame()->GetView();
    RenderWidgetHostView* new_view = new_contents->GetMainFrame()->GetView();
    if (old_view && new_view)
      new_view->SetBackgroundColor(old_view->background_color());
  }

  int index = tab_strip_model_->GetIndexOfWebContents(old_contents);
  DCHECK_NE(TabStripModel::kNoTab, index);
  tab_strip_model_->ReplaceWebContentsAt(index, new_contents);
}
