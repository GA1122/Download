void Browser::OpenPluginsTabAndActivate() {
  OpenURL(GURL(chrome::kChromeUIPluginsURL), GURL(),
          NEW_FOREGROUND_TAB, PageTransition::LINK);
  window_->Activate();
}
