void CardUnmaskPromptViews::FadeOutView::PaintChildren(
    const PaintContext& context) {
  if (opacity_ > 0.99)
    return views::View::PaintChildren(context);

  gfx::Canvas* canvas = context.canvas();
  canvas->SaveLayerAlpha(0xff * opacity_);
  views::View::PaintChildren(context);
  canvas->Restore();
}
