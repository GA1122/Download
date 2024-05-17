content::KeyboardEventProcessingResult BrowserView::PreHandleKeyboardEvent(
    const NativeWebKeyboardEvent& event) {
  if ((event.GetType() != blink::WebInputEvent::kRawKeyDown) &&
      (event.GetType() != blink::WebInputEvent::kKeyUp)) {
    return content::KeyboardEventProcessingResult::NOT_HANDLED;
  }

  views::FocusManager* focus_manager = GetFocusManager();
  DCHECK(focus_manager);

  if (focus_manager->shortcut_handling_suspended())
    return content::KeyboardEventProcessingResult::NOT_HANDLED;

  ui::Accelerator accelerator =
      ui::GetAcceleratorFromNativeWebKeyboardEvent(event);


  if (browser_->is_app()) {
    return content::KeyboardEventProcessingResult::NOT_HANDLED;
  }

#if defined(OS_CHROMEOS)
  if (ash_util::IsAcceleratorDeprecated(accelerator)) {
    return (event.GetType() == blink::WebInputEvent::kRawKeyDown)
               ? content::KeyboardEventProcessingResult::NOT_HANDLED_IS_SHORTCUT
               : content::KeyboardEventProcessingResult::NOT_HANDLED;
  }
#endif   

  content::KeyboardEventProcessingResult result =
      frame_->PreHandleKeyboardEvent(event);
  if (result != content::KeyboardEventProcessingResult::NOT_HANDLED)
    return result;

#if defined(OS_CHROMEOS)
  if (event.os_event && event.os_event->IsKeyEvent() &&
      ash_util::WillAshProcessAcceleratorForEvent(
          *event.os_event->AsKeyEvent())) {
    return content::KeyboardEventProcessingResult::HANDLED_DONT_UPDATE_EVENT;
  }
#endif

  int id;
  if (!FindCommandIdForAccelerator(accelerator, &id)) {
    if (focus_manager->ProcessAccelerator(accelerator))
      return content::KeyboardEventProcessingResult::HANDLED;
    return content::KeyboardEventProcessingResult::NOT_HANDLED;
  }

  chrome::BrowserCommandController* controller = browser_->command_controller();
  if (controller->IsReservedCommandOrKey(id, event)) {
    UpdateAcceleratorMetrics(accelerator, id);
    return focus_manager->ProcessAccelerator(accelerator)
               ? content::KeyboardEventProcessingResult::HANDLED
               : content::KeyboardEventProcessingResult::NOT_HANDLED;
  }

  DCHECK_EQ(event.GetType(), blink::WebInputEvent::kRawKeyDown);
  return content::KeyboardEventProcessingResult::NOT_HANDLED_IS_SHORTCUT;
}
