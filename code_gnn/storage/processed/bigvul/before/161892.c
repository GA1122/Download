void PrintRenderFrameHelper::DidFailProvisionalLoad(
    const blink::WebURLError& error) {
  DidFinishLoad();
}
