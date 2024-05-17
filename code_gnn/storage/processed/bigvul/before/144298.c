content::WebContents* LoginDisplayHostWebUI::GetOobeWebContents() const {
  if (!login_view_)
    return nullptr;
  return login_view_->GetWebContents();
}
