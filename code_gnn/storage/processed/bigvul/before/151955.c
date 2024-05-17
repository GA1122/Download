void RenderFrameHostImpl::GetAuthenticator(
    blink::mojom::AuthenticatorRequest request) {
#if !defined(OS_ANDROID)
  if (base::FeatureList::IsEnabled(features::kWebAuth)) {
    if (base::CommandLine::ForCurrentProcess()->HasSwitch(
            switches::kEnableWebAuthTestingAPI)) {
      ScopedVirtualAuthenticatorEnvironment::GetInstance();
    }

    BindAuthenticatorRequest(std::move(request));
  }
#else
  GetJavaInterfaces()->GetInterface(std::move(request));
#endif   
}
