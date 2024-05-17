void ComponentUpdaterPolicyTest::SetUpOnMainThread() {
  const auto config = component_updater::MakeChromeComponentUpdaterConfigurator(
      base::CommandLine::ForCurrentProcess(), g_browser_process->local_state());
  const auto urls = config->UpdateUrl();
  ASSERT_EQ(1u, urls.size());
  post_interceptor_ = std::make_unique<update_client::URLLoaderPostInterceptor>(
      urls, &https_server_);

  https_server_.StartAcceptingConnections();
  PolicyTest::SetUpOnMainThread();
}
