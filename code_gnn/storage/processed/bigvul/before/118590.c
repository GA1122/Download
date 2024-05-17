void AppLauncherHandler::HandleGenerateAppForLink(const base::ListValue* args) {
  std::string url;
  CHECK(args->GetString(0, &url));
  GURL launch_url(url);

  base::string16 title;
  CHECK(args->GetString(1, &title));

  double page_index;
  CHECK(args->GetDouble(2, &page_index));
  AppSorting* app_sorting =
      ExtensionPrefs::Get(extension_service_->profile())->app_sorting();
  const syncer::StringOrdinal& page_ordinal =
      app_sorting->PageIntegerAsStringOrdinal(static_cast<size_t>(page_index));

  Profile* profile = Profile::FromWebUI(web_ui());
  FaviconService* favicon_service =
      FaviconServiceFactory::GetForProfile(profile, Profile::EXPLICIT_ACCESS);
  if (!favicon_service) {
    LOG(ERROR) << "No favicon service";
    return;
  }

  scoped_ptr<AppInstallInfo> install_info(new AppInstallInfo());
  install_info->title = title;
  install_info->app_url = launch_url;
  install_info->page_ordinal = page_ordinal;

  favicon_service->GetFaviconImageForURL(
      FaviconService::FaviconForURLParams(
          launch_url, favicon_base::FAVICON, gfx::kFaviconSize),
      base::Bind(&AppLauncherHandler::OnFaviconForApp,
                 base::Unretained(this),
                 base::Passed(&install_info)),
      &cancelable_task_tracker_);
}
