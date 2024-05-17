DialogNotification::DialogNotification(Type type,
                                       const base::string16& display_text)
    : type_(type),
      display_text_(display_text),
      checked_(false) {
  std::vector<base::string16> pieces;
  base::SplitStringDontTrim(display_text, kRangeSeparator, &pieces);
  if (pieces.size() > 1) {
    size_t start = pieces[0].size();
    size_t end = start + pieces[1].size();
    link_range_ = gfx::Range(start, end);
    display_text_ = JoinString(pieces, base::string16());
  }
}
