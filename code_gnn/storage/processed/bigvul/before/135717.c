void GranularityStrategyTest::TestDirectionShrink() {
  Selection().MoveRangeSelectionExtent(word_middles_[4]);
  EXPECT_EQ_SELECTED_TEXT("pqr iiinmni");
  IntPoint p = word_middles_[4];
  p.Move(letter_pos_[28].X() - letter_pos_[29].X(), 0);
  Selection().MoveRangeSelectionExtent(p);
  EXPECT_EQ_SELECTED_TEXT("pqr iiinmn");
  p.Move(letter_pos_[27].X() - letter_pos_[28].X(), 0);
  Selection().MoveRangeSelectionExtent(p);
  EXPECT_EQ_SELECTED_TEXT("pqr iiinm");
  p.Move(letter_pos_[26].X() - letter_pos_[27].X(), 0);
  Selection().MoveRangeSelectionExtent(p);
  EXPECT_EQ_SELECTED_TEXT("pqr iiin");
  p.Move(letter_pos_[27].X() - letter_pos_[26].X(), 0);
  Selection().MoveRangeSelectionExtent(p);
  EXPECT_EQ_SELECTED_TEXT("pqr iiin");
  p.Move(letter_pos_[25].X() - letter_pos_[26].X(), 0);
  Selection().MoveRangeSelectionExtent(p);
  EXPECT_EQ_SELECTED_TEXT("pqr iii");
  p.Move(letter_pos_[24].X() - letter_pos_[25].X(), 0);
  Selection().MoveRangeSelectionExtent(p);
  EXPECT_EQ_SELECTED_TEXT("pqr ii");
  Selection().MoveRangeSelectionExtent(letter_pos_[24]);
  EXPECT_EQ_SELECTED_TEXT("pqr ii");
  Selection().MoveRangeSelectionExtent(letter_pos_[25]);
  EXPECT_EQ_SELECTED_TEXT("pqr iii");

  Selection().MoveRangeSelectionExtent(letter_pos_[22]);
  EXPECT_EQ_SELECTED_TEXT("pqr ");
  p = letter_pos_[22];
  p.Move(1, 0);
  Selection().MoveRangeSelectionExtent(p);
  EXPECT_EQ_SELECTED_TEXT("pqr ");
  Selection().MoveRangeSelectionExtent(letter_pos_[23]);
  EXPECT_EQ_SELECTED_TEXT("pqr i");
}
