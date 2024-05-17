void TestRenderFrame::NavigateWithError(
    const CommonNavigationParams& common_params,
    const CommitNavigationParams& commit_params,
    int error_code,
    const base::Optional<std::string>& error_page_content) {
  if (!IsPerNavigationMojoInterfaceEnabled()) {
    CommitFailedNavigation(common_params, commit_params,
                           false  , error_code,
                           error_page_content, nullptr, base::DoNothing());
  } else {
    BindNavigationClient(
        mojo::MakeRequestAssociatedWithDedicatedPipe(&mock_navigation_client_));
    mock_navigation_client_->CommitFailedNavigation(
        common_params, commit_params, false  ,
        error_code, error_page_content, nullptr,
        base::BindOnce(&MockFrameHost::DidCommitProvisionalLoad,
                       base::Unretained(mock_frame_host_.get())));
  }
}
