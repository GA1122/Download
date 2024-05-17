void LoginDisplayHostWebUI::LoadURL(const GURL& url) {
  InitLoginWindowAndView();
  content::WebContentsObserver::Observe(login_view_->GetWebContents());
  login_view_->LoadURL(url);
}
