void RenderWidgetHostViewAura::OnPaint(gfx::Canvas* canvas) {
  paint_canvas_ = canvas;
  BackingStore* backing_store = host_->GetBackingStore(true);
  paint_canvas_ = NULL;
  if (backing_store) {
    static_cast<BackingStoreAura*>(backing_store)->SkiaShowRect(gfx::Point(),
                                                                canvas);
  } else if (aura::Env::GetInstance()->render_white_bg()) {
    canvas->DrawColor(SK_ColorWHITE);
  }
}
