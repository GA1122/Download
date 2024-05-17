 void FrameImpl::CreateView(
    fidl::InterfaceRequest<fuchsia::ui::viewsv1token::ViewOwner> view_owner,
    fidl::InterfaceRequest<fuchsia::sys::ServiceProvider> services) {
  ui::PlatformWindowInitProperties properties;
  properties.view_owner_request = std::move(view_owner);

  window_tree_host_ =
      std::make_unique<aura::WindowTreeHostPlatform>(std::move(properties));
  window_tree_host_->InitHost();
  window_tree_host_->window()->SetLayoutManager(new LayoutManagerImpl());
  window_tree_host_->window()->AddChild(web_contents_->GetNativeView());
  window_tree_host_->window()->Show();
  window_tree_host_->Show();
  web_contents_->GetNativeView()->Show();
}
