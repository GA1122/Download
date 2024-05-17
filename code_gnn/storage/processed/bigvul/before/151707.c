void Browser::OnIsPageTranslatedChanged(content::WebContents* source) {
  DCHECK(source);
  if (tab_strip_model_->GetActiveWebContents() == source) {
    window_->SetTranslateIconToggled(
        ChromeTranslateClient::FromWebContents(
            source)->GetLanguageState().IsPageTranslated());
  }
}
