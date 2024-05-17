WebKit::WebPageVisibilityState RenderView::visibilityState() const {
  WebKit::WebPageVisibilityState current_state = is_hidden() ?
      WebKit::WebPageVisibilityStateHidden :
      WebKit::WebPageVisibilityStateVisible;
  WebKit::WebPageVisibilityState override_state = current_state;
  if (content::GetContentClient()->renderer()->
          ShouldOverridePageVisibilityState(this,
                                            &override_state))
    return override_state;
  return current_state;
}
