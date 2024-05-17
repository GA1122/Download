BrowserWindowGtk::BrowserWindowGtk(Browser* browser)
    :  window_(NULL),
       window_has_shown_(false),
       window_container_(NULL),
       window_vbox_(NULL),
       render_area_vbox_(NULL),
       render_area_floating_container_(NULL),
       render_area_event_box_(NULL),
       toolbar_border_(NULL),
       browser_(browser),
       state_(GDK_WINDOW_STATE_WITHDRAWN),
       devtools_dock_side_(DEVTOOLS_DOCK_SIDE_BOTTOM),
       devtools_window_(NULL),
       contents_hsplit_(NULL),
       contents_vsplit_(NULL),
       frame_cursor_(NULL),
       is_active_(false),
       show_state_after_show_(ui::SHOW_STATE_DEFAULT),
       suppress_window_raise_(false),
       accel_group_(NULL),
       fullscreen_exit_bubble_type_(
           FEB_TYPE_BROWSER_FULLSCREEN_EXIT_INSTRUCTION) {
}
