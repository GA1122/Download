void AutofillPopupViewViews::DrawAutofillEntry(gfx::Canvas* canvas,
                                               int index,
                                               const gfx::Rect& entry_rect) {
  canvas->FillRect(
      entry_rect,
      GetNativeTheme()->GetSystemColor(
          controller_->GetBackgroundColorIDForRow(index)));

  const bool is_rtl = controller_->IsRTL();
  const int text_align =
      is_rtl ? gfx::Canvas::TEXT_ALIGN_RIGHT : gfx::Canvas::TEXT_ALIGN_LEFT;
  gfx::Rect value_rect = entry_rect;
  value_rect.Inset(AutofillPopupLayoutModel::kEndPadding, 0);

  bool icon_on_the_right = !is_rtl;
  int x_align_left = icon_on_the_right ? value_rect.right() : value_rect.x();

  int row_height = controller_->layout_model().GetRowBounds(index).height();
  const gfx::ImageSkia image = controller_->layout_model().GetIconImage(index);
  if (!image.isNull()) {
    int icon_y = entry_rect.y() + (row_height - image.height()) / 2;

    int icon_x_align_left =
        icon_on_the_right ? x_align_left - image.width() : x_align_left;

    canvas->DrawImageInt(image, icon_x_align_left, icon_y);

      x_align_left =
          icon_x_align_left +
          (is_rtl ? image.width() + AutofillPopupLayoutModel::kIconPadding
                  : -AutofillPopupLayoutModel::kIconPadding);
  }

  const int value_width = gfx::GetStringWidth(
      controller_->GetElidedValueAt(index),
      controller_->layout_model().GetValueFontListForRow(index));
  int value_x_align_left =
      is_rtl ? value_rect.right() - value_width : value_rect.x();

  canvas->DrawStringRectWithFlags(
      controller_->GetElidedValueAt(index),
      controller_->layout_model().GetValueFontListForRow(index),
      GetNativeTheme()->GetSystemColor(
          controller_->layout_model().GetValueFontColorIDForRow(index)),
      gfx::Rect(value_x_align_left, value_rect.y(), value_width,
                value_rect.height()),
      text_align);

  if (!controller_->GetSuggestionAt(index).label.empty()) {
    const int label_width = gfx::GetStringWidth(
        controller_->GetElidedLabelAt(index),
        controller_->layout_model().GetLabelFontListForRow(index));
    int label_x_align_left = x_align_left + (is_rtl ? 0 : -label_width);

    canvas->DrawStringRectWithFlags(
        controller_->GetElidedLabelAt(index),
        controller_->layout_model().GetLabelFontListForRow(index),
        GetNativeTheme()->GetSystemColor(
            ui::NativeTheme::kColorId_ResultsTableDimmedText),
        gfx::Rect(label_x_align_left, entry_rect.y(), label_width,
                  entry_rect.height()),
        text_align);
  }
}
