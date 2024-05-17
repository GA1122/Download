const gfx::Image* OmniboxPopupViewGtk::IconForMatch(
    const AutocompleteMatch& match,
    bool selected,
    bool is_selected_keyword) {
  const SkBitmap* bitmap = model_->GetIconIfExtensionMatch(match);
  if (bitmap) {
    if (!ContainsKey(images_, bitmap)) {
      images_[bitmap] = new gfx::Image(gfx::GdkPixbufFromSkBitmap(bitmap));
    }
    return images_[bitmap];
  }

  int icon;
  if (is_selected_keyword)
    icon = IDR_OMNIBOX_TTS;
  else if (match.starred)
    icon = IDR_OMNIBOX_STAR;
  else
    icon = AutocompleteMatch::TypeToIcon(match.type);

  if (selected) {
    switch (icon) {
      case IDR_OMNIBOX_EXTENSION_APP:
        icon = IDR_OMNIBOX_EXTENSION_APP_DARK;
        break;
      case IDR_OMNIBOX_HTTP:
        icon = IDR_OMNIBOX_HTTP_DARK;
        break;
      case IDR_OMNIBOX_HISTORY:
        icon = IDR_OMNIBOX_HISTORY_DARK;
        break;
      case IDR_OMNIBOX_SEARCH:
        icon = IDR_OMNIBOX_SEARCH_DARK;
        break;
      case IDR_OMNIBOX_STAR:
        icon = IDR_OMNIBOX_STAR_DARK;
        break;
      case IDR_OMNIBOX_TTS:
        icon = IDR_OMNIBOX_TTS_DARK;
        break;
      default:
        NOTREACHED();
        break;
    }
  }

  return theme_service_->GetImageNamed(icon);
}
