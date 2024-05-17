void Browser::RendererUnresponsive(
    WebContents* source,
    const content::WebContentsUnresponsiveState& unresponsive_state) {
  int index = tab_strip_model_->GetIndexOfWebContents(source);
  DCHECK_NE(TabStripModel::kNoTab, index);
  if (tab_strip_model_->IsTabBlocked(index))
    return;

  TabDialogs::FromWebContents(source)->ShowHungRendererDialog(
      unresponsive_state);
}
