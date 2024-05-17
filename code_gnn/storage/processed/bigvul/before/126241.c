void Browser::SwapTabContents(content::WebContents* old_contents,
                              content::WebContents* new_contents) {
  int index = tab_strip_model_->GetIndexOfWebContents(old_contents);
  DCHECK_NE(TabStripModel::kNoTab, index);
  TabContents* new_tab_contents = TabContents::FromWebContents(new_contents);
  tab_strip_model_->ReplaceTabContentsAt(index, new_tab_contents);
}
