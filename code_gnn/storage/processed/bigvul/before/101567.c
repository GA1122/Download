void Browser::AddNewContents(TabContents* source,
                             TabContents* new_contents,
                             WindowOpenDisposition disposition,
                             const gfx::Rect& initial_pos,
                             bool user_gesture) {
  DCHECK(disposition != SAVE_TO_DISK);
  DCHECK(disposition != CURRENT_TAB);

  TabContentsWrapper* source_wrapper = NULL;
  BlockedContentTabHelper* source_blocked_content = NULL;
  TabContentsWrapper* new_wrapper =
      TabContentsWrapper::GetCurrentWrapperForContents(new_contents);
  if (!new_wrapper)
    new_wrapper = new TabContentsWrapper(new_contents);
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

    RenderViewHost* view = new_contents->render_view_host();
    view->Send(new ViewMsg_DisassociateFromPopupCount(view->routing_id()));
  }

  browser::NavigateParams params(this, new_wrapper);
  params.source_contents =
      source ? tabstrip_model()->GetTabContentsAt(
                   tabstrip_model()->GetWrapperIndex(source))
             : NULL;
  params.disposition = disposition;
  params.window_bounds = initial_pos;
  params.window_action = browser::NavigateParams::SHOW_WINDOW;
  params.user_gesture = user_gesture;
  browser::Navigate(&params);
}
