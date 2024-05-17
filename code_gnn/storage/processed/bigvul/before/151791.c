BackgroundLoaderOffliner::GetVisibleSecurityState(
    content::WebContents* web_contents) {
  SecurityStateTabHelper::CreateForWebContents(web_contents);
  SecurityStateTabHelper* helper =
      SecurityStateTabHelper::FromWebContents(web_contents);
  DCHECK(helper);
  return helper->GetVisibleSecurityState();
}
