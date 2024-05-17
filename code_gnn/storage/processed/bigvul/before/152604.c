void TestRenderFrame::Navigate(const CommonNavigationParams& common_params,
                               const CommitNavigationParams& commit_params) {
  Navigate(network::ResourceResponseHead(), common_params, commit_params);
}
