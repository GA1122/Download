void BrowserLauncherItemController::UpdateLauncher(TabContents* tab) {
  if (type() == TYPE_APP_PANEL)
    return;   

  if (!tab)
    return;   

  int item_index = launcher_model()->ItemIndexByID(launcher_id());
  if (item_index == -1)
    return;

  ash::LauncherItem item = launcher_model()->items()[item_index];
  if (type() == TYPE_EXTENSION_PANEL) {
    if (!favicon_loader_.get() ||
        favicon_loader_->web_contents() != tab->web_contents()) {
      favicon_loader_.reset(
          new LauncherFaviconLoader(this, tab->web_contents()));
    }
    extensions::TabHelper* extensions_tab_helper =
        extensions::TabHelper::FromWebContents(tab->web_contents());
    gfx::ImageSkia new_image = gfx::ImageSkia(favicon_loader_->GetFavicon());
    if (new_image.isNull() && extensions_tab_helper->GetExtensionAppIcon())
      new_image = gfx::ImageSkia(*extensions_tab_helper->GetExtensionAppIcon());
    if (!new_image.isNull())
      item.image = new_image;
    else if (item.image.isNull())
      item.image = extensions::Extension::GetDefaultIcon(true);
  } else {
    DCHECK_EQ(TYPE_TABBED, type());
    ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
    FaviconTabHelper* favicon_tab_helper =
        FaviconTabHelper::FromWebContents(tab->web_contents());
    if (favicon_tab_helper->ShouldDisplayFavicon()) {
      item.image = favicon_tab_helper->GetFavicon().AsImageSkia();
      if (item.image.isNull()) {
        item.image = *rb.GetImageSkiaNamed(IDR_DEFAULT_FAVICON);
      }
    } else {
      item.image = *rb.GetImageSkiaNamed(IDR_DEFAULT_FAVICON);
    }
  }
  launcher_model()->Set(item_index, item);
}
