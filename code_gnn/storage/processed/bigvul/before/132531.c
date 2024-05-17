void LayoutTestContentBrowserClient::GetAdditionalNavigatorConnectServices(
    const scoped_refptr<NavigatorConnectContext>& context) {
  context->AddFactory(
      make_scoped_ptr(new LayoutTestNavigatorConnectServiceFactory));
}
