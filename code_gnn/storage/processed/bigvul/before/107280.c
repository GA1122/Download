void Browser::AddNewContents(TabContents* source,
                             TabContents* new_contents,
                             WindowOpenDisposition disposition,
                             const gfx::Rect& initial_pos,
                             bool user_gesture) {
  DCHECK(disposition != SAVE_TO_DISK);
  DCHECK(disposition != CURRENT_TAB);

#if defined(OS_CHROMEOS)
  if (disposition == NEW_POPUP) {
    GdkScreen* screen = gdk_screen_get_default();
    int max_width = gdk_screen_get_width(screen) * kPopupMaxWidthFactor;
    int max_height = gdk_screen_get_height(screen) * kPopupMaxHeightFactor;
    if (initial_pos.width() > max_width || initial_pos.width() == 0 ||
        initial_pos.height() > max_height || initial_pos.height() == 0) {
      disposition = NEW_FOREGROUND_TAB;
    }
  }
#endif

  TabContentsWrapper* wrapper = new TabContentsWrapper(new_contents);
  browser::NavigateParams params(this, wrapper);
  params.source_contents =
      tabstrip_model()->GetTabContentsAt(
          tabstrip_model()->GetWrapperIndex(source));
  params.disposition = disposition;
  params.window_bounds = initial_pos;
  params.show_window = true;
  browser::Navigate(&params);
}
