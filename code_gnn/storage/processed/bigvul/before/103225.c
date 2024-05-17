void Browser::Zoom(PageZoom::Function zoom_function) {
  static const UserMetricsAction kActions[] = {
                      UserMetricsAction("ZoomMinus"),
                      UserMetricsAction("ZoomNormal"),
                      UserMetricsAction("ZoomPlus")
                      };

  UserMetrics::RecordAction(kActions[zoom_function - PageZoom::ZOOM_OUT],
                            profile_);
  TabContentsWrapper* tab_contents = GetSelectedTabContentsWrapper();
  tab_contents->render_view_host()->Zoom(zoom_function);
}
