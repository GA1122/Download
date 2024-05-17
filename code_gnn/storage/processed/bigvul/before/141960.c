void AutofillPopupViewViews::OnPaint(gfx::Canvas* canvas) {
  if (!controller_)
    return;

  canvas->DrawColor(GetNativeTheme()->GetSystemColor(
      ui::NativeTheme::kColorId_ResultsTableNormalBackground));
  OnPaintBorder(canvas);

  DCHECK_EQ(controller_->GetLineCount(), child_count());
  for (int i = 0; i < controller_->GetLineCount(); ++i) {
    gfx::Rect line_rect = controller_->layout_model().GetRowBounds(i);

    if (controller_->GetSuggestionAt(i).frontend_id ==
        POPUP_ITEM_ID_SEPARATOR) {
      canvas->FillRect(line_rect,
                       GetNativeTheme()->GetSystemColor(
                           ui::NativeTheme::kColorId_ResultsTableDimmedText));
    } else {
      DrawAutofillEntry(canvas, i, line_rect);
    }
  }
}
