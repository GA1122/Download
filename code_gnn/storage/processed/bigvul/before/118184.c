SuggestionState::SuggestionState(
    bool visible,
    const base::string16& vertically_compact_text,
    const base::string16& horizontally_compact_text,
    const gfx::Image& icon,
    const base::string16& extra_text,
    const gfx::Image& extra_icon)
    : visible(visible),
      vertically_compact_text(vertically_compact_text),
      horizontally_compact_text(horizontally_compact_text),
      icon(icon),
      extra_text(extra_text),
      extra_icon(extra_icon) {}
