void AddA11yStrings(content::WebUIDataSource* html_source) {
#if defined(OS_CHROMEOS)
  int selectToSpeakDescription =
      (base::CommandLine::ForCurrentProcess()->HasSwitch(
          chromeos::switches::kEnableExperimentalAccessibilityFeatures))
          ? IDS_SETTINGS_ACCESSIBILITY_SELECT_TO_SPEAK_DESCRIPTION_FLAGS
          : IDS_SETTINGS_ACCESSIBILITY_SELECT_TO_SPEAK_DESCRIPTION;
#endif

  LocalizedString localized_strings[] = {
    {"a11yPageTitle", IDS_SETTINGS_ACCESSIBILITY},
    {"a11yWebStore", IDS_SETTINGS_ACCESSIBILITY_WEB_STORE},
    {"moreFeaturesLink", IDS_SETTINGS_MORE_FEATURES_LINK},
    {"moreFeaturesLinkDescription",
     IDS_SETTINGS_MORE_FEATURES_LINK_DESCRIPTION},
#if defined(OS_CHROMEOS)
    {"optionsInMenuLabel", IDS_SETTINGS_OPTIONS_IN_MENU_LABEL},
    {"largeMouseCursorLabel", IDS_SETTINGS_LARGE_MOUSE_CURSOR_LABEL},
    {"largeMouseCursorSizeLabel", IDS_SETTINGS_LARGE_MOUSE_CURSOR_SIZE_LABEL},
    {"largeMouseCursorSizeDefaultLabel",
     IDS_SETTINGS_LARGE_MOUSE_CURSOR_SIZE_DEFAULT_LABEL},
    {"largeMouseCursorSizeLargeLabel",
     IDS_SETTINGS_LARGE_MOUSE_CURSOR_SIZE_LARGE_LABEL},
    {"highContrastLabel", IDS_SETTINGS_HIGH_CONTRAST_LABEL},
    {"stickyKeysLabel", IDS_SETTINGS_STICKY_KEYS_LABEL},
    {"chromeVoxLabel", IDS_SETTINGS_CHROMEVOX_LABEL},
    {"chromeVoxOptionsLabel", IDS_SETTINGS_CHROMEVOX_OPTIONS_LABEL},
    {"screenMagnifierLabel", IDS_SETTINGS_SCREEN_MAGNIFIER_LABEL},
    {"screenMagnifierZoomLabel", IDS_SETTINGS_SCREEN_MAGNIFIER_ZOOM_LABEL},
    {"dockedMagnifierLabel", IDS_SETTINGS_DOCKED_MAGNIFIER_LABEL},
    {"dockedMagnifierZoomLabel", IDS_SETTINGS_DOCKED_MAGNIFIER_ZOOM_LABEL},
    {"screenMagnifierZoom2x", IDS_SETTINGS_SCREEN_MAGNIFIER_ZOOM_2_X},
    {"screenMagnifierZoom4x", IDS_SETTINGS_SCREEN_MAGNIFIER_ZOOM_4_X},
    {"screenMagnifierZoom6x", IDS_SETTINGS_SCREEN_MAGNIFIER_ZOOM_6_X},
    {"screenMagnifierZoom8x", IDS_SETTINGS_SCREEN_MAGNIFIER_ZOOM_8_X},
    {"screenMagnifierZoom10x", IDS_SETTINGS_SCREEN_MAGNIFIER_ZOOM_10_X},
    {"screenMagnifierZoom12x", IDS_SETTINGS_SCREEN_MAGNIFIER_ZOOM_12_X},
    {"screenMagnifierZoom14x", IDS_SETTINGS_SCREEN_MAGNIFIER_ZOOM_14_X},
    {"screenMagnifierZoom16x", IDS_SETTINGS_SCREEN_MAGNIFIER_ZOOM_16_X},
    {"screenMagnifierZoom18x", IDS_SETTINGS_SCREEN_MAGNIFIER_ZOOM_18_X},
    {"screenMagnifierZoom20x", IDS_SETTINGS_SCREEN_MAGNIFIER_ZOOM_20_X},
    {"tapDraggingLabel", IDS_SETTINGS_TAP_DRAGGING_LABEL},
    {"clickOnStopLabel", IDS_SETTINGS_CLICK_ON_STOP_LABEL},
    {"delayBeforeClickLabel", IDS_SETTINGS_DELAY_BEFORE_CLICK_LABEL},
    {"delayBeforeClickExtremelyShort",
     IDS_SETTINGS_DELAY_BEFORE_CLICK_EXTREMELY_SHORT},
    {"delayBeforeClickVeryShort", IDS_SETTINGS_DELAY_BEFORE_CLICK_VERY_SHORT},
    {"delayBeforeClickShort", IDS_SETTINGS_DELAY_BEFORE_CLICK_SHORT},
    {"delayBeforeClickLong", IDS_SETTINGS_DELAY_BEFORE_CLICK_LONG},
    {"delayBeforeClickVeryLong", IDS_SETTINGS_DELAY_BEFORE_CLICK_VERY_LONG},
    {"onScreenKeyboardLabel", IDS_SETTINGS_ON_SCREEN_KEYBOARD_LABEL},
    {"monoAudioLabel", IDS_SETTINGS_MONO_AUDIO_LABEL},
    {"a11yExplanation", IDS_SETTINGS_ACCESSIBILITY_EXPLANATION},
    {"caretHighlightLabel",
     IDS_SETTINGS_ACCESSIBILITY_CARET_HIGHLIGHT_DESCRIPTION},
    {"cursorHighlightLabel",
     IDS_SETTINGS_ACCESSIBILITY_CURSOR_HIGHLIGHT_DESCRIPTION},
    {"focusHighlightLabel",
     IDS_SETTINGS_ACCESSIBILITY_FOCUS_HIGHLIGHT_DESCRIPTION},
    {"selectToSpeakTitle", IDS_SETTINGS_ACCESSIBILITY_SELECT_TO_SPEAK_TITLE},
    {"selectToSpeakDescription", selectToSpeakDescription},
    {"selectToSpeakOptionsLabel",
     IDS_SETTINGS_ACCESSIBILITY_SELECT_TO_SPEAK_OPTIONS_LABEL},
    {"switchAccessLabel", IDS_SETTINGS_ACCESSIBILITY_SWITCH_ACCESS_DESCRIPTION},
    {"switchAccessOptionsLabel",
     IDS_SETTINGS_ACCESSIBILITY_SWITCH_ACCESS_OPTIONS_LABEL},
    {"manageAccessibilityFeatures",
     IDS_SETTINGS_ACCESSIBILITY_MANAGE_ACCESSIBILITY_FEATURES},
    {"textToSpeechHeading", IDS_SETTINGS_ACCESSIBILITY_TEXT_TO_SPEECH_HEADING},
    {"displayHeading", IDS_SETTINGS_ACCESSIBILITY_DISPLAY_HEADING},
    {"displaySettingsTitle", IDS_SETTINGS_ACCESSIBILITY_DISPLAY_SETTINGS_TITLE},
    {"displaySettingsDescription",
     IDS_SETTINGS_ACCESSIBILITY_DISPLAY_SETTINGS_DESCRIPTION},
    {"appearanceSettingsTitle",
     IDS_SETTINGS_ACCESSIBILITY_APPEARANCE_SETTINGS_TITLE},
    {"appearanceSettingsDescription",
     IDS_SETTINGS_ACCESSIBILITY_APPEARANCE_SETTINGS_DESCRIPTION},
    {"keyboardHeading", IDS_SETTINGS_ACCESSIBILITY_KEYBOARD_HEADING},
    {"keyboardSettingsTitle",
     IDS_SETTINGS_ACCESSIBILITY_KEYBOARD_SETTINGS_TITLE},
    {"keyboardSettingsDescription",
     IDS_SETTINGS_ACCESSIBILITY_KEYBOARD_SETTINGS_DESCRIPTION},
    {"mouseAndTouchpadHeading",
     IDS_SETTINGS_ACCESSIBILITY_MOUSE_AND_TOUCHPAD_HEADING},
    {"mouseSettingsTitle", IDS_SETTINGS_ACCESSIBILITY_MOUSE_SETTINGS_TITLE},
    {"mouseSettingsDescription",
     IDS_SETTINGS_ACCESSIBILITY_MOUSE_SETTINGS_DESCRIPTION},
    {"audioHeading", IDS_SETTINGS_ACCESSIBILITY_AUDIO_HEADING},
    {"additionalFeaturesTitle",
     IDS_SETTINGS_ACCESSIBILITY_ADDITIONAL_FEATURES_TITLE},
#endif
  };
  AddLocalizedStringsBulk(html_source, localized_strings,
                          arraysize(localized_strings));

#if defined(OS_CHROMEOS)
  html_source->AddString("a11yLearnMoreUrl",
                         chrome::kChromeAccessibilityHelpURL);

  html_source->AddBoolean(
      "showExperimentalA11yFeatures",
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          chromeos::switches::kEnableExperimentalAccessibilityFeatures));

  html_source->AddBoolean("dockedMagnifierFeatureEnabled",
                          ash::features::IsDockedMagnifierEnabled());
#endif
}
