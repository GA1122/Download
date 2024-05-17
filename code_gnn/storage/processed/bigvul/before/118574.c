void CreateShortcutsInApplicationsMenu(Profile* profile,
                                       const Extension* app) {
  web_app::ShortcutLocations creation_locations;
  creation_locations.applications_menu_location =
      web_app::APP_MENU_LOCATION_SUBDIR_CHROMEAPPS;
  web_app::CreateShortcuts(
      web_app::SHORTCUT_CREATION_AUTOMATED, creation_locations, profile, app);
}
