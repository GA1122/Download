  void TestCreateSettingsMenuForExtension(const FilePath::StringType& path,
                                          Extension::Location location,
                                          const std::string& homepage_url,
                                          const std::string& options_page) {
    DictionaryValue extra_value;
    if (!homepage_url.empty()) {
      extra_value.SetString(extension_manifest_keys::kHomepageURL,
                            homepage_url);
    }
    if (!options_page.empty()) {
      extra_value.SetString(extension_manifest_keys::kOptionsPage,
                            options_page);
    }
    scoped_refptr<Extension> extension = CreateExtension(
        path, location, extra_value);

    Panel* panel = CreatePanel(
        web_app::GenerateApplicationNameFromExtensionId(extension->id()));

    scoped_ptr<PanelSettingsMenuModel> settings_menu_model(
        new PanelSettingsMenuModel(panel));

    MenuItem expected_panel_menu_items[] = {
        { PanelSettingsMenuModel::COMMAND_NAME, false },
        { -1, false },   
        { PanelSettingsMenuModel::COMMAND_CONFIGURE, false },
        { PanelSettingsMenuModel::COMMAND_DISABLE, false },
        { PanelSettingsMenuModel::COMMAND_UNINSTALL, false },
        { -1, false },   
        { PanelSettingsMenuModel::COMMAND_MANAGE, true }
    };
    if (!homepage_url.empty())
      expected_panel_menu_items[0].enabled = true;
    if (!options_page.empty())
      expected_panel_menu_items[2].enabled = true;
    if (location != Extension::EXTERNAL_POLICY_DOWNLOAD) {
      expected_panel_menu_items[3].enabled = true;
      expected_panel_menu_items[4].enabled = true;
    }
    ValidateSettingsMenuItems(settings_menu_model.get(),
                              arraysize(expected_panel_menu_items),
                              expected_panel_menu_items);

    panel->Close();
  }
