bool OmniboxViewViews::UnapplySteadyStateElisions(UnelisionGesture gesture) {
  if (IsSelectAll() && gesture != UnelisionGesture::HOME_KEY_PRESSED)
    return false;

  size_t start, end;
  GetSelectionBounds(&start, &end);

  base::string16 original_text = GetText();
  base::string16 original_selected_text = GetSelectedText();
  if (!model()->Unelide(false  ))
    return false;

  size_t offset = GetText().find(original_text);
  if (offset != base::string16::npos) {
    if (start != end && gesture == UnelisionGesture::MOUSE_RELEASE &&
        !model()->ClassifiesAsSearch(original_selected_text)) {
      if (start != 0)
        start += offset;
      if (end != 0)
        end += offset;
    } else {
      start += offset;
      end += offset;
    }

    OffsetDoubleClickWord(offset);
  }

  SelectRange(gfx::Range(start, end));
  return true;
}
