 void ComponentControllerImpl::CreateView(
     fidl::InterfaceRequest<fuchsia::ui::viewsv1token::ViewOwner> view_owner,
     fidl::InterfaceRequest<fuchsia::sys::ServiceProvider> services) {
  DCHECK(frame_);
  DCHECK(!view_is_bound_);

  frame_->CreateView(std::move(view_owner), std::move(services));
  view_is_bound_ = true;
}
