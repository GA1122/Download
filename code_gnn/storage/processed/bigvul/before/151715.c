WebContents* Browser::OpenURLFromTab(WebContents* source,
                                     const OpenURLParams& params) {
  if (is_devtools()) {
    DevToolsWindow* window = DevToolsWindow::AsDevToolsWindow(source);
    DCHECK(window);
    return window->OpenURLFromTab(source, params);
  }

  chrome::NavigateParams nav_params(this, params.url, params.transition);
  FillNavigateParamsFromOpenURLParams(&nav_params, params);
  nav_params.source_contents = source;
  nav_params.tabstrip_add_types = TabStripModel::ADD_NONE;
  if (params.user_gesture)
    nav_params.window_action = chrome::NavigateParams::SHOW_WINDOW;
  nav_params.user_gesture = params.user_gesture;

  PopupBlockerTabHelper* popup_blocker_helper = NULL;
  if (source)
    popup_blocker_helper = PopupBlockerTabHelper::FromWebContents(source);

  if (popup_blocker_helper) {
    if ((params.disposition == WindowOpenDisposition::NEW_POPUP ||
         params.disposition == WindowOpenDisposition::NEW_FOREGROUND_TAB ||
         params.disposition == WindowOpenDisposition::NEW_BACKGROUND_TAB ||
         params.disposition == WindowOpenDisposition::NEW_WINDOW) &&
        !params.user_gesture &&
        !base::CommandLine::ForCurrentProcess()->HasSwitch(
            switches::kDisablePopupBlocking)) {
      if (popup_blocker_helper->MaybeBlockPopup(nav_params,
                                                WebWindowFeatures())) {
        return NULL;
      }
    }
  }

  chrome::Navigate(&nav_params);

  return nav_params.target_contents;
}
