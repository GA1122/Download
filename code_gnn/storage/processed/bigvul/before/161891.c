PrepareFrameAndViewForPrint::CreateURLLoaderFactory() {
  return blink::Platform::Current()->CreateDefaultURLLoaderFactory();
}
