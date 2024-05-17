void ChromeContentBrowserClient::OverrideWebkitPrefs(
    RenderViewHost* rvh, const GURL& url, WebPreferences* web_prefs) {
  Profile* profile = Profile::FromBrowserContext(
      rvh->GetProcess()->GetBrowserContext());
  PrefService* prefs = profile->GetPrefs();

  FillFontFamilyMap(prefs, prefs::kWebKitStandardFontFamilyMap,
                    &web_prefs->standard_font_family_map);
  FillFontFamilyMap(prefs, prefs::kWebKitFixedFontFamilyMap,
                    &web_prefs->fixed_font_family_map);
  FillFontFamilyMap(prefs, prefs::kWebKitSerifFontFamilyMap,
                    &web_prefs->serif_font_family_map);
  FillFontFamilyMap(prefs, prefs::kWebKitSansSerifFontFamilyMap,
                    &web_prefs->sans_serif_font_family_map);
  FillFontFamilyMap(prefs, prefs::kWebKitCursiveFontFamilyMap,
                    &web_prefs->cursive_font_family_map);
  FillFontFamilyMap(prefs, prefs::kWebKitFantasyFontFamilyMap,
                    &web_prefs->fantasy_font_family_map);
  FillFontFamilyMap(prefs, prefs::kWebKitPictographFontFamilyMap,
                    &web_prefs->pictograph_font_family_map);

  web_prefs->default_font_size =
      prefs->GetInteger(prefs::kWebKitDefaultFontSize);
  web_prefs->default_fixed_font_size =
      prefs->GetInteger(prefs::kWebKitDefaultFixedFontSize);
  web_prefs->minimum_font_size =
      prefs->GetInteger(prefs::kWebKitMinimumFontSize);
  web_prefs->minimum_logical_font_size =
      prefs->GetInteger(prefs::kWebKitMinimumLogicalFontSize);

  web_prefs->default_encoding = prefs->GetString(prefs::kDefaultCharset);

  web_prefs->javascript_can_open_windows_automatically =
      prefs->GetBoolean(prefs::kWebKitJavascriptCanOpenWindowsAutomatically);
  web_prefs->dom_paste_enabled =
      prefs->GetBoolean(prefs::kWebKitDomPasteEnabled);
  web_prefs->shrinks_standalone_images_to_fit =
      prefs->GetBoolean(prefs::kWebKitShrinksStandaloneImagesToFit);
  const DictionaryValue* inspector_settings =
      prefs->GetDictionary(prefs::kWebKitInspectorSettings);
  if (inspector_settings) {
    for (DictionaryValue::key_iterator iter(inspector_settings->begin_keys());
         iter != inspector_settings->end_keys(); ++iter) {
      std::string value;
      if (inspector_settings->GetStringWithoutPathExpansion(*iter, &value))
          web_prefs->inspector_settings.push_back(
              std::make_pair(*iter, value));
    }
  }
  web_prefs->tabs_to_links = prefs->GetBoolean(prefs::kWebkitTabsToLinks);

  if (!prefs->GetBoolean(prefs::kWebKitJavascriptEnabled))
    web_prefs->javascript_enabled = false;
  if (!prefs->GetBoolean(prefs::kWebKitWebSecurityEnabled))
    web_prefs->web_security_enabled = false;
  if (!prefs->GetBoolean(prefs::kWebKitPluginsEnabled))
    web_prefs->plugins_enabled = false;
  if (!prefs->GetBoolean(prefs::kWebKitJavaEnabled))
    web_prefs->java_enabled = false;
  web_prefs->loads_images_automatically =
      prefs->GetBoolean(prefs::kWebKitLoadsImagesAutomatically);

  if (prefs->GetBoolean(prefs::kDisable3DAPIs))
    web_prefs->experimental_webgl_enabled = false;

  web_prefs->memory_info_enabled =
      prefs->GetBoolean(prefs::kEnableMemoryInfo);
  web_prefs->allow_displaying_insecure_content =
      prefs->GetBoolean(prefs::kWebKitAllowDisplayingInsecureContent);
  web_prefs->allow_running_insecure_content =
      prefs->GetBoolean(prefs::kWebKitAllowRunningInsecureContent);
#if defined(OS_ANDROID)
  web_prefs->font_scale_factor =
      static_cast<float>(prefs->GetDouble(prefs::kWebKitFontScaleFactor));
  web_prefs->force_enable_zoom =
      prefs->GetBoolean(prefs::kWebKitForceEnableZoom);
#endif
  web_prefs->password_echo_enabled = browser_defaults::kPasswordEchoEnabled;

  UserStyleSheetWatcher* user_style_sheet_watcher =
      UserStyleSheetWatcherFactory::GetForProfile(profile);
  if (user_style_sheet_watcher) {
    web_prefs->user_style_sheet_enabled = true;
    web_prefs->user_style_sheet_location =
        user_style_sheet_watcher->user_style_sheet();
  } else {
    web_prefs->user_style_sheet_enabled = false;
  }

  web_prefs->asynchronous_spell_checking_enabled =
#if defined(OS_MACOSX)
      false;
#else
      true;
#endif
  web_prefs->unified_textchecker_enabled =
      web_prefs->asynchronous_spell_checking_enabled;

  web_prefs->uses_universal_detector =
      prefs->GetBoolean(prefs::kWebKitUsesUniversalDetector);
  web_prefs->text_areas_are_resizable =
      prefs->GetBoolean(prefs::kWebKitTextAreasAreResizable);
  web_prefs->hyperlink_auditing_enabled =
      prefs->GetBoolean(prefs::kEnableHyperlinkAuditing);

  web_prefs->default_encoding =
      CharacterEncoding::GetCanonicalEncodingNameByAliasName(
          web_prefs->default_encoding);
  if (web_prefs->default_encoding.empty()) {
    prefs->ClearPref(prefs::kDefaultCharset);
    web_prefs->default_encoding = prefs->GetString(prefs::kDefaultCharset);
  }
  DCHECK(!web_prefs->default_encoding.empty());

  if (content::IsForceCompositingModeEnabled())
    web_prefs->force_compositing_mode = true;

  WebContents* web_contents = WebContents::FromRenderViewHost(rvh);
  chrome::ViewType view_type = chrome::GetViewType(web_contents);
  ExtensionService* service =
      extensions::ExtensionSystem::Get(profile)->extension_service();
  if (service) {
    const GURL& url = rvh->GetSiteInstance()->GetSiteURL();
    const Extension* extension = service->extensions()->GetByID(url.host());
    if (url.SchemeIs(extensions::kExtensionScheme)) {
      extension_webkit_preferences::SetPreferences(
          extension, view_type, web_prefs);
    }
  }

  if (view_type == chrome::VIEW_TYPE_NOTIFICATION) {
    web_prefs->allow_scripts_to_close_windows = true;
  } else if (view_type == chrome::VIEW_TYPE_BACKGROUND_CONTENTS) {
    web_prefs->force_compositing_mode = false;
    web_prefs->accelerated_compositing_enabled = false;
  }

#if defined(FILE_MANAGER_EXTENSION)
  if (url.spec() == chrome::kChromeUIFileManagerURL) {
    web_prefs->accelerated_compositing_enabled = true;
    web_prefs->accelerated_2d_canvas_enabled = true;
  }
#endif
}
