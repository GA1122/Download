TabContents* Browser::OpenApplication(
    Profile* profile,
    const Extension* extension,
    extension_misc::LaunchContainer container,
    WindowOpenDisposition disposition) {
  TabContents* tab = NULL;
  ExtensionPrefs* prefs = profile->GetExtensionService()->extension_prefs();
  prefs->SetActiveBit(extension->id(), true);

  UMA_HISTOGRAM_ENUMERATION("Extensions.AppLaunchContainer", container, 100);

  switch (container) {
    case extension_misc::LAUNCH_WINDOW:
    case extension_misc::LAUNCH_PANEL:
      tab = Browser::OpenApplicationWindow(profile, extension, container,
                                           GURL(), NULL);
      break;
    case extension_misc::LAUNCH_TAB: {
      tab = Browser::OpenApplicationTab(profile, extension, disposition);
      break;
    }
    default:
      NOTREACHED();
      break;
  }
  return tab;
}
