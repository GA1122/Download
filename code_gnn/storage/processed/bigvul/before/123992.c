WebKit::WebPageVisibilityState RenderViewImpl::visibilityState() const {
  WebKit::WebPageVisibilityState current_state = is_hidden() ?
      WebKit::WebPageVisibilityStateHidden :
      WebKit::WebPageVisibilityStateVisible;
  WebKit::WebPageVisibilityState override_state = current_state;
  if (GetContentClient()->renderer()->
          ShouldOverridePageVisibilityState(this,
                                            &override_state))
    return override_state;
  return current_state;
}
