blink::WebNavigationPolicy RenderFrameImpl::decidePolicyForNavigation(
    const NavigationPolicyInfo& info) {
  DCHECK(!frame_ || frame_ == info.frame);
  return DecidePolicyForNavigation(this, info);
}
