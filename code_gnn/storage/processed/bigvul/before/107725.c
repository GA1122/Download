void NavigationState::RunPerNavigationInterfaceCommitNavigationCallback(
    std::unique_ptr<::FrameHostMsg_DidCommitProvisionalLoad_Params> params,
    mojom::DidCommitProvisionalLoadInterfaceParamsPtr interface_params) {
  DCHECK(IsPerNavigationMojoInterfaceEnabled());
  if (per_navigation_mojo_interface_commit_callback_)
    std::move(per_navigation_mojo_interface_commit_callback_)
        .Run(std::move(params), std::move(interface_params));
  navigation_client_.reset();
}
