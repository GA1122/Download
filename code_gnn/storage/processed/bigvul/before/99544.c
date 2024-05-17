void BookmarkDrag::BeginDrag(Profile* profile,
                             const std::vector<const BookmarkNode*>& nodes) {
  new BookmarkDrag(profile, nodes);
}
