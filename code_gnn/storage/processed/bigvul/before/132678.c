void BlinkTestRunner::SetGamepadProvider(
    scoped_ptr<RendererGamepadProvider> provider) {
  SetMockGamepadProvider(provider.Pass());
}
