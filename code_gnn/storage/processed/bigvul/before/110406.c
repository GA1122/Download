void WebUILoginView::Init(views::Widget* login_window) {
  login_window_ = login_window;
  webui_login_ = new views::WebView(ProfileManager::GetDefaultProfile());
  AddChildView(webui_login_);

  WebContents* web_contents =
      WebContents::Create(ProfileManager::GetDefaultProfile(),
                          NULL,
                          MSG_ROUTING_NONE,
                          NULL,
                          NULL);
  tab_contents_.reset(new TabContents(web_contents));
  webui_login_->SetWebContents(web_contents);

  web_contents->SetDelegate(this);
  renderer_preferences_util::UpdateFromSystemSettings(
      web_contents->GetMutableRendererPrefs(),
      ProfileManager::GetDefaultProfile());

  registrar_.Add(this,
                 content::NOTIFICATION_RENDER_VIEW_HOST_CREATED_FOR_TAB,
                 content::Source<WebContents>(web_contents));
}
