int RenderProcessHostImpl::GetActiveViewCount() {
  int num_active_views = 0;
  for (RenderWidgetHostsIterator iter = GetRenderWidgetHostsIterator();
       !iter.IsAtEnd();
       iter.Advance()) {
    const RenderWidgetHost* widget = iter.GetCurrentValue();
    DCHECK(widget);
    if (!widget)
      continue;

    if (!widget->IsRenderView()) {
      num_active_views++;
      continue;
    }

    RenderViewHost* rvh =
        RenderViewHost::From(const_cast<RenderWidgetHost*>(widget));
    if (!static_cast<RenderViewHostImpl*>(rvh)->is_swapped_out())
      num_active_views++;
  }
  return num_active_views;
}
