void SendRoutedTouchTapSequence(content::WebContents* web_contents,
                                gfx::Point point) {
  RenderWidgetHostViewAura* rwhva = static_cast<RenderWidgetHostViewAura*>(
      web_contents->GetRenderWidgetHostView());
  ui::TouchEvent touch_start(
      ui::ET_TOUCH_PRESSED, point, base::TimeTicks::Now(),
      ui::PointerDetails(ui::EventPointerType::POINTER_TYPE_TOUCH, 0));
  rwhva->OnTouchEvent(&touch_start);
  ui::TouchEvent touch_end(
      ui::ET_TOUCH_RELEASED, point, base::TimeTicks::Now(),
      ui::PointerDetails(ui::EventPointerType::POINTER_TYPE_TOUCH, 0));
  rwhva->OnTouchEvent(&touch_end);
}
