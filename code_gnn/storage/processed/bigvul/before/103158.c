void Browser::OpenPluginsTabAndActivate() {
  OpenURL(GURL(chrome::kAboutPluginsURL), GURL(),
          NEW_FOREGROUND_TAB, PageTransition::LINK);
  window_->Activate();
}
