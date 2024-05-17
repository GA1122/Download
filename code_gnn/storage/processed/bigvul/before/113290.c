void PanelBrowserView::DrawAttention(bool draw_attention) {
  DCHECK((panel_->attention_mode() & Panel::USE_PANEL_ATTENTION) != 0);

  if (is_drawing_attention_ == draw_attention)
    return;
  is_drawing_attention_ = draw_attention;
  GetFrameView()->SchedulePaint();

  if ((panel_->attention_mode() & Panel::USE_SYSTEM_ATTENTION) != 0)
    ::BrowserView::FlashFrame(draw_attention);
}
