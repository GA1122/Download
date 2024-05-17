RenderWidgetHostViewGuest::RenderWidgetHostViewGuest(
    RenderWidgetHost* widget_host,
    BrowserPluginGuest* guest)
    : host_(RenderWidgetHostImpl::From(widget_host)),
      is_hidden_(false),
      guest_(guest) {
  host_->SetView(this);
}
