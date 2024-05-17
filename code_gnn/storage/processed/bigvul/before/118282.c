void AutofillDialogViews::SuggestedButton::OnPaint(gfx::Canvas* canvas) {
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  const gfx::Insets insets = GetInsets();
  canvas->DrawImageInt(*rb.GetImageSkiaNamed(ResourceIDForState()),
                       insets.left(), insets.top());
  views::Painter::PaintFocusPainter(this, canvas, focus_painter());
}
