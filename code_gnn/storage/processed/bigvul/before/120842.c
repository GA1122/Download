void LogHistogramValue(SyncPromoUI::Source source, int action) {
  switch (source) {
    case SyncPromoUI::SOURCE_START_PAGE:
      UMA_HISTOGRAM_ENUMERATION("Signin.StartPageActions", action,
                                one_click_signin::HISTOGRAM_MAX);
      break;
    case SyncPromoUI::SOURCE_NTP_LINK:
      UMA_HISTOGRAM_ENUMERATION("Signin.NTPLinkActions", action,
                                one_click_signin::HISTOGRAM_MAX);
      break;
    case SyncPromoUI::SOURCE_MENU:
      UMA_HISTOGRAM_ENUMERATION("Signin.MenuActions", action,
                                one_click_signin::HISTOGRAM_MAX);
      break;
    case SyncPromoUI::SOURCE_SETTINGS:
      UMA_HISTOGRAM_ENUMERATION("Signin.SettingsActions", action,
                                one_click_signin::HISTOGRAM_MAX);
      break;
    case SyncPromoUI::SOURCE_EXTENSION_INSTALL_BUBBLE:
      UMA_HISTOGRAM_ENUMERATION("Signin.ExtensionInstallBubbleActions", action,
                                one_click_signin::HISTOGRAM_MAX);
      break;
    case SyncPromoUI::SOURCE_WEBSTORE_INSTALL:
      UMA_HISTOGRAM_ENUMERATION("Signin.WebstoreInstallActions", action,
                                one_click_signin::HISTOGRAM_MAX);
      break;
    case SyncPromoUI::SOURCE_APP_LAUNCHER:
      UMA_HISTOGRAM_ENUMERATION("Signin.AppLauncherActions", action,
                                one_click_signin::HISTOGRAM_MAX);
      break;
    case SyncPromoUI::SOURCE_APPS_PAGE_LINK:
      UMA_HISTOGRAM_ENUMERATION("Signin.AppsPageLinkActions", action,
                                one_click_signin::HISTOGRAM_MAX);
      break;
    default:
      NOTREACHED() << "Invalid Source";
      return;
  }
  UMA_HISTOGRAM_ENUMERATION("Signin.AllAccessPointActions", action,
                            one_click_signin::HISTOGRAM_MAX);
}
