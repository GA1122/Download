bool IsStartOfDifferentDirection(const InlineBox* inline_box) {
  InlineBox* prev_box = inline_box->PrevLeafChild();
  if (!prev_box)
    return true;
  if (prev_box->Direction() == inline_box->Direction())
    return true;
  DCHECK_NE(prev_box->BidiLevel(), inline_box->BidiLevel());
  return prev_box->BidiLevel() > inline_box->BidiLevel();
}
