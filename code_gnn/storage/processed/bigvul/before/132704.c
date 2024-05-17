void LayoutTestContentRendererClient::WebTestProxyCreated(
    RenderView* render_view,
    WebTestProxyBase* proxy) {
  BlinkTestRunner* test_runner = new BlinkTestRunner(render_view);
  test_runner->set_proxy(proxy);
  if (!LayoutTestRenderProcessObserver::GetInstance()->test_delegate()) {
    LayoutTestRenderProcessObserver::GetInstance()->SetTestDelegate(
        test_runner);
  }
  proxy->SetInterfaces(
      LayoutTestRenderProcessObserver::GetInstance()->test_interfaces());
  test_runner->proxy()->SetDelegate(
      LayoutTestRenderProcessObserver::GetInstance()->test_delegate());
}
