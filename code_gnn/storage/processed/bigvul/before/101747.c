void Browser::OpenMobilePlanTabAndActivate() {
  OpenURL(GURL(chrome::kChromeUIMobileSetupURL), GURL(),
          NEW_FOREGROUND_TAB, PageTransition::LINK);
  window_->Activate();
}
