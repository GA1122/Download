bool Textfield::GetWordLookupDataAtPoint(const gfx::Point& point,
                                         gfx::DecoratedText* decorated_word,
                                         gfx::Point* baseline_point) {
  return GetRenderText()->GetWordLookupDataAtPoint(point, decorated_word,
                                                   baseline_point);
}
