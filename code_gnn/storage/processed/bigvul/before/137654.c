void RenderThreadImpl::SampleGamepads(blink::WebGamepads* data) {
  webkit_platform_support_->sampleGamepads(*data);
}
