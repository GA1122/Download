void GranularityStrategyTest::TestDirectionExpand() {
  Selection().MoveRangeSelectionExtent(letter_pos_[20]);
  EXPECT_EQ_SELECTED_TEXT("pq");
  Selection().MoveRangeSelectionExtent(letter_pos_[20]);
  EXPECT_EQ_SELECTED_TEXT("pq");
  Selection().MoveRangeSelectionExtent(letter_pos_[21]);
  EXPECT_EQ_SELECTED_TEXT("pqr");
  Selection().MoveRangeSelectionExtent(letter_pos_[22]);
  EXPECT_EQ_SELECTED_TEXT("pqr ");
  Selection().MoveRangeSelectionExtent(letter_pos_[24]);
  EXPECT_EQ_SELECTED_TEXT("pqr ");
  IntPoint p = word_middles_[4];
  p.Move(-1, 0);
  Selection().MoveRangeSelectionExtent(p);
  EXPECT_EQ_SELECTED_TEXT("pqr ");
  p.Move(1, 0);
  Selection().MoveRangeSelectionExtent(p);
  EXPECT_EQ_SELECTED_TEXT("pqr stuvwi");
  Selection().MoveRangeSelectionExtent(letter_pos_[27]);
  EXPECT_EQ_SELECTED_TEXT("pqr stuvwi");
  Selection().MoveRangeSelectionExtent(letter_pos_[28]);
  EXPECT_EQ_SELECTED_TEXT("pqr stuvwi");
  Selection().MoveRangeSelectionExtent(letter_pos_[29]);
  EXPECT_EQ_SELECTED_TEXT("pqr stuvwi ");
  int y = letter_pos_[29].Y();
  for (int x = letter_pos_[29].X() + 1; x < word_middles_[5].X(); x++) {
    Selection().MoveRangeSelectionExtent(IntPoint(x, y));
    Selection().MoveRangeSelectionExtent(IntPoint(x, y));
    EXPECT_EQ_SELECTED_TEXT("pqr stuvwi ");
  }
  Selection().MoveRangeSelectionExtent(word_middles_[5]);
  EXPECT_EQ_SELECTED_TEXT("pqr stuvwi inm");
  p = word_middles_[6];
  p.Move(-1, 0);
  Selection().MoveRangeSelectionExtent(p);
  EXPECT_EQ_SELECTED_TEXT("pqr stuvwi inm ");
  p.Move(1, 0);
  Selection().MoveRangeSelectionExtent(p);
  EXPECT_EQ_SELECTED_TEXT("pqr stuvwi inm mnii");
}
