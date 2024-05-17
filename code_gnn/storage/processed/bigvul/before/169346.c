void SimulateTouchPressAt(WebContents* web_contents, const gfx::Point& point) {
  ui::TouchEvent touch(
      ui::ET_TOUCH_PRESSED, point, base::TimeTicks(),
      ui::PointerDetails(ui::EventPointerType::POINTER_TYPE_TOUCH, 0));
  static_cast<RenderWidgetHostViewAura*>(
      web_contents->GetRenderWidgetHostView())
      ->OnTouchEvent(&touch);
}
