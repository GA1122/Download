void Browser::AddNewContents(TabContents* source,
                             TabContents* new_contents,
                             WindowOpenDisposition disposition,
                             const gfx::Rect& initial_pos,
                             bool user_gesture) {
  DCHECK(disposition != SAVE_TO_DISK);
  DCHECK(disposition != CURRENT_TAB);

  TabContentsWrapper* source_wrapper = NULL;
  BlockedContentTabHelper* source_blocked_content = NULL;
  TabContentsWrapper* new_wrapper = new TabContentsWrapper(new_contents);
  if (source) {
    source_wrapper = TabContentsWrapper::GetCurrentWrapperForContents(source);
    source_blocked_content = source_wrapper->blocked_content_tab_helper();
  }

  if (source_wrapper) {
    if (source_blocked_content->all_contents_blocked()) {
      source_blocked_content->AddTabContents(new_wrapper,
                                             disposition,
                                             initial_pos,
                                             user_gesture);
      return;
    }

    if ((disposition == NEW_POPUP) && !user_gesture &&
        !CommandLine::ForCurrentProcess()->HasSwitch(
            switches::kDisablePopupBlocking)) {
      GetConstrainingContentsWrapper(source_wrapper)->
          blocked_content_tab_helper()->
              AddPopup(new_wrapper, initial_pos, user_gesture);
      return;
    }

    new_contents->DisassociateFromPopupCount();
  }

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

  browser::NavigateParams params(this, new_wrapper);
  params.source_contents =
      source ? tabstrip_model()->GetTabContentsAt(
                   tabstrip_model()->GetWrapperIndex(source))
             : NULL;
  params.disposition = disposition;
  params.window_bounds = initial_pos;
  if (disposition == NEW_POPUP && !user_gesture)
    params.window_action = browser::NavigateParams::SHOW_WINDOW_INACTIVE;
  else
    params.window_action = browser::NavigateParams::SHOW_WINDOW;
  browser::Navigate(&params);

  if (source) {
    NotificationService::current()->Notify(
      NotificationType::TAB_ADDED,
      Source<TabContentsDelegate>(source->delegate()),
      Details<TabContents>(source));
  }
}
