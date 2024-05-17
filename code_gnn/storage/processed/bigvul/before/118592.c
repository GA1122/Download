void AppLauncherHandler::HandleLaunchApp(const base::ListValue* args) {
  std::string extension_id;
  CHECK(args->GetString(0, &extension_id));
  double source = -1.0;
  CHECK(args->GetDouble(1, &source));
  std::string url;
  if (args->GetSize() > 2)
    CHECK(args->GetString(2, &url));

  extension_misc::AppLaunchBucket launch_bucket =
      static_cast<extension_misc::AppLaunchBucket>(
          static_cast<int>(source));
  CHECK(launch_bucket >= 0 &&
        launch_bucket < extension_misc::APP_LAUNCH_BUCKET_BOUNDARY);

  const Extension* extension =
      extension_service_->GetExtensionById(extension_id, false);

  if (!extension) {
    PromptToEnableApp(extension_id);
    return;
  }

  Profile* profile = extension_service_->profile();

  WindowOpenDisposition disposition = args->GetSize() > 3 ?
        webui::GetDispositionFromClick(args, 3) : CURRENT_TAB;
  if (extension_id != extension_misc::kWebStoreAppId) {
    CHECK_NE(launch_bucket, extension_misc::APP_LAUNCH_BUCKET_INVALID);
    CoreAppLauncherHandler::RecordAppLaunchType(launch_bucket,
                                                extension->GetType());
  } else {
    CoreAppLauncherHandler::RecordWebStoreLaunch();
  }

  if (disposition == NEW_FOREGROUND_TAB || disposition == NEW_BACKGROUND_TAB ||
      disposition == NEW_WINDOW) {
    AppLaunchParams params(profile, extension,
                           disposition == NEW_WINDOW ?
                               extensions::LAUNCH_CONTAINER_WINDOW :
                               extensions::LAUNCH_CONTAINER_TAB,
                           disposition);
    params.override_url = GURL(url);
    OpenApplication(params);
  } else {
    Browser* browser = chrome::FindBrowserWithWebContents(
        web_ui()->GetWebContents());
    WebContents* old_contents = NULL;
    if (browser)
      old_contents = browser->tab_strip_model()->GetActiveWebContents();

    AppLaunchParams params(profile, extension,
                           old_contents ? CURRENT_TAB : NEW_FOREGROUND_TAB);
    params.override_url = GURL(url);
    WebContents* new_contents = OpenApplication(params);

    if (new_contents != old_contents && browser &&
        browser->tab_strip_model()->count() > 1) {
      chrome::CloseWebContents(browser, old_contents, true);
    }
  }
}
