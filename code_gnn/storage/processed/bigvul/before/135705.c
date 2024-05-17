void GranularityStrategyTest::ParseText(const TextNodeVector& text_nodes) {
  bool word_started = false;
  int word_start_index = 0;
  for (auto& text : text_nodes) {
    int word_start_index_offset = letter_pos_.size();
    String str = text->wholeText();
    for (size_t i = 0; i < str.length(); i++) {
      letter_pos_.push_back(VisiblePositionToContentsPoint(
          CreateVisiblePosition(Position(text, i))));
      char c = str[i];
      if (IsASCIIAlphanumeric(c) && !word_started) {
        word_start_index = i + word_start_index_offset;
        word_started = true;
      } else if (!IsASCIIAlphanumeric(c) && word_started) {
        IntPoint word_middle((letter_pos_[word_start_index].X() +
                              letter_pos_[i + word_start_index_offset].X()) /
                                 2,
                             letter_pos_[word_start_index].Y());
        word_middles_.push_back(word_middle);
        word_started = false;
      }
    }
  }
  if (word_started) {
    const auto& last_node = text_nodes.back();
    int x_end = VisiblePositionToContentsPoint(
                    CreateVisiblePosition(
                        Position(last_node, last_node->wholeText().length())))
                    .X();
    IntPoint word_middle((letter_pos_[word_start_index].X() + x_end) / 2,
                         letter_pos_[word_start_index].Y());
    word_middles_.push_back(word_middle);
  }
}
