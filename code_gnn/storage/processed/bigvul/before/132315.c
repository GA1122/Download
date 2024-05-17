void RenderFrameImpl::didMatchCSS(
    blink::WebLocalFrame* frame,
    const blink::WebVector<blink::WebString>& newly_matching_selectors,
    const blink::WebVector<blink::WebString>& stopped_matching_selectors) {
  DCHECK(!frame_ || frame_ == frame);

  FOR_EACH_OBSERVER(RenderFrameObserver, observers_,
                    DidMatchCSS(newly_matching_selectors,
                                stopped_matching_selectors));
}