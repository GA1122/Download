void RenderFrameHostImpl::GetVirtualAuthenticatorManager(
    blink::test::mojom::VirtualAuthenticatorManagerRequest request) {
#if !defined(OS_ANDROID)
  if (base::FeatureList::IsEnabled(features::kWebAuth)) {
    if (base::CommandLine::ForCurrentProcess()->HasSwitch(
            switches::kEnableWebAuthTestingAPI)) {
      auto* environment_singleton =
          ScopedVirtualAuthenticatorEnvironment::GetInstance();
      environment_singleton->AddBinding(std::move(request));
    }
  }
#endif   
}
