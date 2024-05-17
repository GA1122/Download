void MetalayerMode::OnTouchEvent(ui::TouchEvent* event) {
  if (!feature_enabled())
    return;

  if (!palette_utils::IsInUserSession())
    return;

  if (enabled())
    return;

  if (event->pointer_details().pointer_type !=
      ui::EventPointerType::POINTER_TYPE_PEN)
    return;

  if (event->type() == ui::ET_TOUCH_RELEASED) {
    previous_stroke_end_ = event->time_stamp();
    return;
  }

  if (event->type() != ui::ET_TOUCH_PRESSED)
    return;

  if (event->time_stamp() - previous_stroke_end_ <
      base::TimeDelta::FromMilliseconds(kMaxStrokeGapWhenWritingMs)) {
    return;
  }

  if (!(event->flags() & ui::EF_LEFT_MOUSE_BUTTON))
    return;

  if (palette_utils::PaletteContainsPointInScreen(event->root_location()))
    return;

  if (loading()) {
    ToastData toast(
        kToastId,
        l10n_util::GetStringUTF16(IDS_ASH_STYLUS_TOOLS_METALAYER_TOAST_LOADING),
        kToastDurationMs, base::Optional<base::string16>());
    Shell::Get()->toast_manager()->Show(toast);
  } else {
    delegate()->RecordPaletteOptionsUsage(
        PaletteToolIdToPaletteTrayOptions(GetToolId()),
        PaletteInvocationMethod::SHORTCUT);
    activated_via_button_ = true;
    delegate()->EnableTool(GetToolId());
  }
  event->StopPropagation();
}
