void LoginDisplayHostWebUI::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  LoginDisplayHostCommon::Observe(type, source, details);

  if (chrome::NOTIFICATION_LOGIN_OR_LOCK_WEBUI_VISIBLE == type ||
      chrome::NOTIFICATION_LOGIN_NETWORK_ERROR_SHOWN == type) {
    VLOG(1) << "Login WebUI >> WEBUI_VISIBLE";
    if (waiting_for_wallpaper_load_ && initialize_webui_hidden_) {
      waiting_for_wallpaper_load_ = false;
      ShowWebUI();
    }
    registrar_.Remove(this, chrome::NOTIFICATION_LOGIN_OR_LOCK_WEBUI_VISIBLE,
                      content::NotificationService::AllSources());
    registrar_.Remove(this, chrome::NOTIFICATION_LOGIN_NETWORK_ERROR_SHOWN,
                      content::NotificationService::AllSources());
  } else if (type == chrome::NOTIFICATION_LOGIN_USER_CHANGED &&
             user_manager::UserManager::Get()->IsCurrentUserNew()) {
    registrar_.Remove(this, chrome::NOTIFICATION_LOGIN_USER_CHANGED,
                      content::NotificationService::AllSources());
  } else if (chrome::NOTIFICATION_WALLPAPER_ANIMATION_FINISHED == type) {
    VLOG(1) << "Login WebUI >> wp animation done";
    is_wallpaper_loaded_ = true;
    if (waiting_for_wallpaper_load_) {
      waiting_for_wallpaper_load_ = false;
      if (initialize_webui_hidden_) {
        if (login_window_ && login_view_)
          ShowWebUI();
      } else {
        StartPostponedWebUI();
      }
    }
    registrar_.Remove(this, chrome::NOTIFICATION_WALLPAPER_ANIMATION_FINISHED,
                      content::NotificationService::AllSources());
  }
}
