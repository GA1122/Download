void Browser::OnExtensionUnloaded(content::BrowserContext* browser_context,
                                  const extensions::Extension* extension,
                                  extensions::UnloadedExtensionReason reason) {
  command_controller_->ExtensionStateChanged();

  if (reason != extensions::UnloadedExtensionReason::TERMINATE) {
    for (int i = tab_strip_model_->count() - 1; i >= 0; --i) {
      WebContents* web_contents = tab_strip_model_->GetWebContentsAt(i);

      if ((web_contents->GetURL().SchemeIs(extensions::kExtensionScheme) &&
           web_contents->GetURL().host_piece() == extension->id()) ||
          (extensions::TabHelper::FromWebContents(web_contents)
               ->extension_app() == extension)) {
        tab_strip_model_->CloseWebContentsAt(i, TabStripModel::CLOSE_NONE);
      } else {
        chrome::UnmuteIfMutedByExtension(web_contents, extension->id());
      }
    }
  }
}
