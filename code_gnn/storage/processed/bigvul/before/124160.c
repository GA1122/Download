RenderViewHostManager::RenderViewHostManager(
    RenderViewHostDelegate* render_view_delegate,
    RenderWidgetHostDelegate* render_widget_delegate,
    Delegate* delegate)
    : delegate_(delegate),
      cross_navigation_pending_(false),
      render_view_delegate_(render_view_delegate),
      render_widget_delegate_(render_widget_delegate),
      render_view_host_(NULL),
      pending_render_view_host_(NULL),
      interstitial_page_(NULL) {
}
