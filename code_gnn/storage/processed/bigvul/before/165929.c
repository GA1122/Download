void RenderFrameImpl::RegisterMojoInterfaces() {
  GetAssociatedInterfaceRegistry()->AddInterface(base::Bind(
      &RenderFrameImpl::BindAutoplayConfiguration, weak_factory_.GetWeakPtr()));

  GetAssociatedInterfaceRegistry()->AddInterface(base::Bind(
      &RenderFrameImpl::BindFrameBindingsControl, weak_factory_.GetWeakPtr()));

  GetAssociatedInterfaceRegistry()->AddInterface(
      base::Bind(&RenderFrameImpl::BindFrameNavigationControl,
                 weak_factory_.GetWeakPtr()));

  if (IsPerNavigationMojoInterfaceEnabled()) {
    GetAssociatedInterfaceRegistry()->AddInterface(base::BindRepeating(
        &RenderFrameImpl::BindNavigationClient, weak_factory_.GetWeakPtr()));
  }

  GetAssociatedInterfaceRegistry()->AddInterface(base::BindRepeating(
      &RenderFrameImpl::BindFullscreen, weak_factory_.GetWeakPtr()));

  registry_.AddInterface(base::Bind(&FrameInputHandlerImpl::CreateMojoService,
                                    weak_factory_.GetWeakPtr()));

  registry_.AddInterface(
      base::Bind(&InputTargetClientImpl::BindToRequest,
                 base::Unretained(&input_target_client_impl_)));

  registry_.AddInterface(
      base::Bind(&RenderFrameImpl::BindWidget, weak_factory_.GetWeakPtr()));

  if (!frame_->Parent()) {
    registry_.AddInterface(base::Bind(&ImageDownloaderImpl::CreateMojoService,
                                      base::Unretained(this)));

    GetAssociatedInterfaceRegistry()->AddInterface(base::Bind(
        &RenderFrameImpl::OnHostZoomClientRequest, weak_factory_.GetWeakPtr()));

    registry_.AddInterface(
        base::Bind(&ManifestManager::BindToRequest,
                   base::Unretained(manifest_manager_.get())));
  }
}
