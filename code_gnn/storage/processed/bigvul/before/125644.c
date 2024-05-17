void RenderViewHostImpl::OnDidChangeBodyBackgroundColor(SkColor color) {
  if (GetView())
    GetView()->SetCachedBackgroundColor(color);
}
