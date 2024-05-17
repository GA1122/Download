WebKit::WebGraphicsContext3D* CreateTestContext() {
  ui::TestWebGraphicsContext3D* test_context =
      new ui::TestWebGraphicsContext3D();
  test_context->Initialize();
  return test_context;
}
