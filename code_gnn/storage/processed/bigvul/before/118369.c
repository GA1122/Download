void CardUnmaskPromptViews::FadeOutView::OnPaint(gfx::Canvas* canvas) {
  if (!fade_everything_ || opacity_ > 0.99)
    return views::View::OnPaint(canvas);

  canvas->SaveLayerAlpha(0xff * opacity_);
  views::View::OnPaint(canvas);
  canvas->Restore();
}
