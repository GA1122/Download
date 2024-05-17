void Browser::OnWebContentsInstantSupportDisabled(
    const content::WebContents* web_contents) {
  DCHECK(web_contents);
  if (tab_strip_model_->GetActiveWebContents() == web_contents)
    UpdateToolbar(false);
}
