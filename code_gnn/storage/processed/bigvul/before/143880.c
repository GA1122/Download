ComponentControllerImpl::CreateForRequest(
    WebContentRunner* runner,
    fuchsia::sys::Package package,
    fuchsia::sys::StartupInfo startup_info,
    fidl::InterfaceRequest<fuchsia::sys::ComponentController>
        controller_request) {
  std::unique_ptr<ComponentControllerImpl> result{
      new ComponentControllerImpl(runner)};
  if (!result->BindToRequest(std::move(package), std::move(startup_info),
                             std::move(controller_request))) {
    return nullptr;
  }
  return result;
 }
