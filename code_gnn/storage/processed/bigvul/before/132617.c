void ExportLayoutTestSpecificPreferences(const TestPreferences& from,
                                         WebPreferences* to) {
  to->allow_universal_access_from_file_urls =
      from.allow_universal_access_from_file_urls;
  to->dom_paste_enabled = from.dom_paste_allowed;
  to->javascript_can_access_clipboard = from.java_script_can_access_clipboard;
  to->xss_auditor_enabled = from.xss_auditor_enabled;
  to->editing_behavior = static_cast<EditingBehavior>(from.editing_behavior);
  to->default_font_size = from.default_font_size;
  to->minimum_font_size = from.minimum_font_size;
  to->default_encoding = from.default_text_encoding_name.utf8().data();
  to->javascript_enabled = from.java_script_enabled;
  to->supports_multiple_windows = from.supports_multiple_windows;
  to->loads_images_automatically = from.loads_images_automatically;
  to->plugins_enabled = from.plugins_enabled;
  to->java_enabled = from.java_enabled;
  to->application_cache_enabled = from.offline_web_application_cache_enabled;
  to->tabs_to_links = from.tabs_to_links;
  to->experimental_webgl_enabled = from.experimental_webgl_enabled;
  to->hyperlink_auditing_enabled = from.hyperlink_auditing_enabled;
  to->caret_browsing_enabled = from.caret_browsing_enabled;
  to->allow_displaying_insecure_content =
      from.allow_display_of_insecure_content;
  to->allow_running_insecure_content = from.allow_running_of_insecure_content;
  to->should_respect_image_orientation = from.should_respect_image_orientation;
  to->asynchronous_spell_checking_enabled =
      from.asynchronous_spell_checking_enabled;
  to->allow_file_access_from_file_urls = from.allow_file_access_from_file_urls;
  to->javascript_can_open_windows_automatically =
      from.java_script_can_open_windows_automatically;
  to->web_security_enabled =
      from.web_security_enabled;
  to->disable_reading_from_canvas = from.disable_reading_from_canvas;
  to->strict_mixed_content_checking =
      from.strict_mixed_content_checking;
  to->strict_powerful_feature_restrictions =
      from.strict_powerful_feature_restrictions;
}
