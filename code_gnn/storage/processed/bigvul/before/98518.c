GdkPixbuf* IconForMatch(BrowserThemeProvider* theme,
                        const AutocompleteMatch& match,
                        bool selected) {
  int icon = match.starred ?
      IDR_OMNIBOX_STAR : AutocompleteMatch::TypeToIcon(match.type);
  if (selected) {
    switch (icon) {
      case IDR_OMNIBOX_HTTP:    icon = IDR_OMNIBOX_HTTP_DARK; break;
      case IDR_OMNIBOX_HISTORY: icon = IDR_OMNIBOX_HISTORY_DARK; break;
      case IDR_OMNIBOX_SEARCH:  icon = IDR_OMNIBOX_SEARCH_DARK; break;
      case IDR_OMNIBOX_MORE:    icon = IDR_OMNIBOX_MORE_DARK; break;
      case IDR_OMNIBOX_STAR:    icon = IDR_OMNIBOX_STAR_DARK; break;
      default:                  NOTREACHED(); break;
    }
  }

  return theme->GetPixbufNamed(icon);
}
