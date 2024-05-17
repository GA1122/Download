WebPreferences::WebPreferences()
    : default_font_size(16),
      default_fixed_font_size(13),
      minimum_font_size(0),
      minimum_logical_font_size(6),
      default_encoding("ISO-8859-1"),
      apply_default_device_scale_factor_in_compositor(false),
      apply_page_scale_factor_in_compositor(false),
      per_tile_painting_enabled(false),
      accelerated_animation_enabled(false),
      javascript_enabled(true),
      web_security_enabled(true),
      javascript_can_open_windows_automatically(true),
      loads_images_automatically(true),
      images_enabled(true),
      plugins_enabled(true),
      dom_paste_enabled(false),   
      developer_extras_enabled(false),   
      site_specific_quirks_enabled(false),
      shrinks_standalone_images_to_fit(true),
      uses_universal_detector(false),   
      text_areas_are_resizable(true),
      java_enabled(true),
      allow_scripts_to_close_windows(false),
      uses_page_cache(false),
      page_cache_supports_plugins(false),
      remote_fonts_enabled(true),
      javascript_can_access_clipboard(false),
      xss_auditor_enabled(true),
      dns_prefetching_enabled(true),
      local_storage_enabled(false),
      databases_enabled(false),
      application_cache_enabled(false),
      tabs_to_links(true),
      caret_browsing_enabled(false),
      hyperlink_auditing_enabled(true),
      is_online(true),
      user_style_sheet_enabled(false),
      author_and_user_styles_enabled(true),
      frame_flattening_enabled(false),
      allow_universal_access_from_file_urls(false),
      allow_file_access_from_file_urls(false),
      webaudio_enabled(false),
      experimental_webgl_enabled(false),
      flash_3d_enabled(true),
      flash_stage3d_enabled(false),
      gl_multisampling_enabled(true),
      privileged_webgl_extensions_enabled(false),
      webgl_errors_to_console_enabled(true),
      show_composited_layer_borders(false),
      show_composited_layer_tree(false),
      show_fps_counter(false),
      accelerated_compositing_for_overflow_scroll_enabled(false),
      accelerated_compositing_for_scrollable_frames_enabled(false),
      composited_scrolling_for_frames_enabled(false),
      show_paint_rects(false),
      render_vsync_enabled(true),
      asynchronous_spell_checking_enabled(true),
      unified_textchecker_enabled(false),
      accelerated_compositing_enabled(false),
      force_compositing_mode(false),
      fixed_position_compositing_enabled(false),
      accelerated_compositing_for_3d_transforms_enabled(false),
      accelerated_compositing_for_animation_enabled(false),
      accelerated_compositing_for_video_enabled(false),
      accelerated_2d_canvas_enabled(false),
      deferred_2d_canvas_enabled(false),
      antialiased_2d_canvas_disabled(false),
      accelerated_painting_enabled(false),
      accelerated_filters_enabled(false),
      gesture_tap_highlight_enabled(false),
      accelerated_compositing_for_plugins_enabled(false),
      memory_info_enabled(false),
      fullscreen_enabled(false),
      allow_displaying_insecure_content(true),
      allow_running_insecure_content(false),
      password_echo_enabled(false),
      should_print_backgrounds(false),
      enable_scroll_animator(false),
      visual_word_movement_enabled(false),
      css_sticky_position_enabled(false),
      css_shaders_enabled(false),
      css_variables_enabled(false),
      css_grid_layout_enabled(false),
      touch_enabled(false),
      device_supports_touch(false),
      device_supports_mouse(true),
      touch_adjustment_enabled(true),
      default_tile_width(256),
      default_tile_height(256),
      max_untiled_layer_width(512),
      max_untiled_layer_height(512),
      fixed_position_creates_stacking_context(false),
      sync_xhr_in_documents_enabled(true),
      deferred_image_decoding_enabled(false),
      should_respect_image_orientation(false),
      number_of_cpu_cores(1),
#if defined(OS_MACOSX)
      editing_behavior(EDITING_BEHAVIOR_MAC),
#elif defined(OS_WIN)
      editing_behavior(EDITING_BEHAVIOR_WIN),
#elif defined(OS_POSIX)
      editing_behavior(EDITING_BEHAVIOR_UNIX),
#else
      editing_behavior(EDITING_BEHAVIOR_MAC),
#endif
      supports_multiple_windows(true),
      viewport_enabled(false),
      cookie_enabled(true)
#if defined(OS_ANDROID)
      ,
      text_autosizing_enabled(true),
      font_scale_factor(1.0f),
      force_enable_zoom(false),
      user_gesture_required_for_media_playback(true)
#endif
{
  standard_font_family_map[kCommonScript] =
      ASCIIToUTF16("Times New Roman");
  fixed_font_family_map[kCommonScript] =
      ASCIIToUTF16("Courier New");
  serif_font_family_map[kCommonScript] =
      ASCIIToUTF16("Times New Roman");
  sans_serif_font_family_map[kCommonScript] =
      ASCIIToUTF16("Arial");
  cursive_font_family_map[kCommonScript] =
      ASCIIToUTF16("Script");
  fantasy_font_family_map[kCommonScript] =
      ASCIIToUTF16("Impact");
  pictograph_font_family_map[kCommonScript] =
      ASCIIToUTF16("Times New Roman");
}
