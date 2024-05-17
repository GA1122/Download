void RenderWidgetHostViewAura::ShowContextMenu(
    const ContextMenuParams& params) {
  RenderViewHost* rvh = RenderViewHost::From(host_);
  if (!rvh)
    return;

  RenderViewHostDelegate* delegate = rvh->GetDelegate();
  if (!delegate)
    return;

  RenderViewHostDelegateView* delegate_view = delegate->GetDelegateView();
  if (!delegate_view)
    return;
  delegate_view->ShowContextMenu(GetFocusedFrame(), params);
}
