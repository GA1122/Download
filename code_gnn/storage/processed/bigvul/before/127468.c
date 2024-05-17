void ChromeGeolocationPermissionContext::RequestGeolocationPermission(
    int render_process_id, int render_view_id, int bridge_id,
    const GURL& requesting_frame, base::Callback<void(bool)> callback) {
  if (!BrowserThread::CurrentlyOn(BrowserThread::UI)) {
    BrowserThread::PostTask(
        BrowserThread::UI, FROM_HERE,
        base::Bind(
            &ChromeGeolocationPermissionContext::RequestGeolocationPermission,
            this, render_process_id, render_view_id, bridge_id,
            requesting_frame, callback));
    return;
  }
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

#if defined(OS_ANDROID)
  if (!profile_->GetPrefs()->GetBoolean(prefs::kGeolocationEnabled)) {
    NotifyPermissionSet(render_process_id, render_view_id, bridge_id,
                        requesting_frame, callback, false);
    return;
  }
#endif

  ExtensionService* extension_service = profile_->GetExtensionService();
  if (extension_service) {
    const extensions::Extension* extension =
        extension_service->extensions()->GetExtensionOrAppByURL(
            ExtensionURLInfo(
                WebSecurityOrigin::createFromString(
                    UTF8ToUTF16(requesting_frame.spec())),
                requesting_frame));
    if (extension &&
        extension->HasAPIPermission(extensions::APIPermission::kGeolocation)) {
      if (extension_service->process_map()->Contains(
              extension->id(), render_process_id)) {
        NotifyPermissionSet(render_process_id, render_view_id, bridge_id,
                            requesting_frame, callback, true);
        return;
      }
    }
  }

  WebContents* web_contents =
      tab_util::GetWebContentsByID(render_process_id, render_view_id);
  if (chrome::GetViewType(web_contents) != chrome::VIEW_TYPE_TAB_CONTENTS) {
    LOG(WARNING) << "Attempt to use geolocation tabless renderer: "
                 << render_process_id << "," << render_view_id << ","
                 << bridge_id << " (can't prompt user without a visible tab)";
    NotifyPermissionSet(render_process_id, render_view_id, bridge_id,
                        requesting_frame, callback, false);
    return;
  }

  GURL embedder = web_contents->GetURL();
  if (!requesting_frame.is_valid() || !embedder.is_valid()) {
    LOG(WARNING) << "Attempt to use geolocation from an invalid URL: "
                 << requesting_frame << "," << embedder
                 << " (geolocation is not supported in popups)";
    NotifyPermissionSet(render_process_id, render_view_id, bridge_id,
                        requesting_frame, callback, false);
    return;
  }

  ContentSetting content_setting =
     profile_->GetHostContentSettingsMap()->GetContentSetting(
          requesting_frame,
          embedder,
          CONTENT_SETTINGS_TYPE_GEOLOCATION,
          std::string());
  if (content_setting == CONTENT_SETTING_BLOCK) {
    NotifyPermissionSet(render_process_id, render_view_id, bridge_id,
                        requesting_frame, callback, false);
  } else if (content_setting == CONTENT_SETTING_ALLOW) {
    NotifyPermissionSet(render_process_id, render_view_id, bridge_id,
                        requesting_frame, callback, true);
  } else {   
    geolocation_infobar_queue_controller_->CreateInfoBarRequest(
        render_process_id, render_view_id, bridge_id, requesting_frame,
        embedder, callback);
  }
}
