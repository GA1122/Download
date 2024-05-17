void Browser::OpenSystemTabAndActivate() {
  OpenURL(GURL(chrome::kChromeUISystemInfoURL), GURL(),
          NEW_FOREGROUND_TAB, PageTransition::LINK);
  window_->Activate();
}
