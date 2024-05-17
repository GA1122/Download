static String CreateMarkupInRect(LocalFrame* frame,
                                 const IntPoint& start_point,
                                 const IntPoint& end_point) {
  VisiblePosition start_visible_position =
      VisiblePositionForContentsPoint(start_point, frame);
  VisiblePosition end_visible_position =
      VisiblePositionForContentsPoint(end_point, frame);

  Position start_position = start_visible_position.DeepEquivalent();
  Position end_position = end_visible_position.DeepEquivalent();

  if (!start_position.GetDocument() || !end_position.GetDocument())
    return String();

  if (start_position.CompareTo(end_position) <= 0) {
    return CreateMarkup(start_position, end_position, kAnnotateForInterchange,
                        ConvertBlocksToInlines::kNotConvert,
                        kResolveNonLocalURLs);
  }
  return CreateMarkup(end_position, start_position, kAnnotateForInterchange,
                      ConvertBlocksToInlines::kNotConvert,
                      kResolveNonLocalURLs);
}
