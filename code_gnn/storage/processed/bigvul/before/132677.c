void BlinkTestRunner::SetFocus(WebTestProxyBase* proxy, bool focus) {
  ProxyToRenderViewVisitor visitor(proxy);
  RenderView::ForEach(&visitor);
  if (!visitor.render_view()) {
    NOTREACHED();
    return;
  }

  if (!BlinkTestRunner::Get(focused_view_))
    focused_view_ = NULL;

  if (focus) {
    if (focused_view_ != visitor.render_view()) {
      if (focused_view_)
        SetFocusAndActivate(focused_view_, false);
      SetFocusAndActivate(visitor.render_view(), true);
      focused_view_ = visitor.render_view();
    }
  } else {
    if (focused_view_ == visitor.render_view()) {
      SetFocusAndActivate(visitor.render_view(), false);
      focused_view_ = NULL;
    }
  }
}
