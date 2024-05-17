void AppLauncherHandler::OnFaviconForApp(
    scoped_ptr<AppInstallInfo> install_info,
    const favicon_base::FaviconImageResult& image_result) {
  scoped_ptr<WebApplicationInfo> web_app(new WebApplicationInfo());
  web_app->title = install_info->title;
  web_app->app_url = install_info->app_url;

  if (!image_result.image.IsEmpty()) {
    WebApplicationInfo::IconInfo icon;
    icon.data = image_result.image.AsBitmap();
    icon.width = icon.data.width();
    icon.height = icon.data.height();
    web_app->icons.push_back(icon);
  }

  scoped_refptr<CrxInstaller> installer(
      CrxInstaller::CreateSilent(extension_service_));
  installer->set_error_on_unsupported_requirements(true);
  installer->set_page_ordinal(install_info->page_ordinal);
  installer->InstallWebApp(*web_app);
  attempted_bookmark_app_install_ = true;
}
