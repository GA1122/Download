WebContents* Browser::OpenURLFromTab(WebContents* source,
                                     const OpenURLParams& params) {
  if (is_devtools()) {
    DevToolsWindow* window = DevToolsWindow::AsDevToolsWindow(source);
    DCHECK(window);
    return window->OpenURLFromTab(source, params);
  }

  NavigateParams nav_params(this, params.url, params.transition);
  nav_params.FillNavigateParamsFromOpenURLParams(params);
  nav_params.source_contents = source;
  nav_params.tabstrip_add_types = TabStripModel::ADD_NONE;
  if (params.user_gesture)
    nav_params.window_action = NavigateParams::SHOW_WINDOW;
  nav_params.user_gesture = params.user_gesture;
  nav_params.blob_url_loader_factory = params.blob_url_loader_factory;
  bool is_popup = source && PopupBlockerTabHelper::ConsiderForPopupBlocking(
                                params.disposition);
  if (is_popup && PopupBlockerTabHelper::MaybeBlockPopup(
                      source, base::Optional<GURL>(), &nav_params, &params,
                      blink::mojom::WindowFeatures())) {
    return nullptr;
  }

  Navigate(&nav_params);

  if (is_popup && nav_params.navigated_or_inserted_contents)
    PopupTracker::CreateForWebContents(
        nav_params.navigated_or_inserted_contents, source);

  return nav_params.navigated_or_inserted_contents;
}
