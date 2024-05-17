  void PerformClick(int x, int y) {
    content::WebContents* contents =
        browser()->tab_strip_model()->GetActiveWebContents();
    blink::WebMouseEvent click_event(
        blink::WebInputEvent::kMouseDown, blink::WebInputEvent::kNoModifiers,
        blink::WebInputEvent::GetStaticTimeStampForTests());
    click_event.button = blink::WebMouseEvent::Button::kLeft;
    click_event.click_count = 1;
    click_event.SetPositionInWidget(x, y);
    contents->GetRenderViewHost()->GetWidget()->ForwardMouseEvent(click_event);
    click_event.SetType(blink::WebInputEvent::kMouseUp);
    contents->GetRenderViewHost()->GetWidget()->ForwardMouseEvent(click_event);
  }
