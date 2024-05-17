void Browser::SetTabContentBlocked(content::WebContents* web_contents,
                                   bool blocked) {
  int index = tab_strip_model_->GetIndexOfWebContents(web_contents);
  if (index == TabStripModel::kNoTab) {
    NOTREACHED();
    return;
  }
  tab_strip_model_->SetTabBlocked(index, blocked);
  command_controller_->PrintingStateChanged();
  command_controller_->FullscreenStateChanged();
  if (!blocked && chrome::GetActiveWebContents(this) == web_contents)
    web_contents->Focus();
}
