void ExtensionInfoBar::RunMenu(View* source, const gfx::Point& pt) {
  const Extension* extension = delegate_->extension_host()->extension();
  if (!extension->ShowConfigureContextMenus())
    return;

  if (!options_menu_contents_.get()) {
    Browser* browser = BrowserView::GetBrowserViewForNativeWindow(
        platform_util::GetTopLevel(source->GetWidget()->GetNativeView()))->
            browser();
    options_menu_contents_ = new ExtensionContextMenuModel(
        extension, browser, NULL);
  }

  options_menu_menu_.reset(new views::Menu2(options_menu_contents_.get()));
  options_menu_menu_->RunMenuAt(pt, views::Menu2::ALIGN_TOPLEFT);
}
