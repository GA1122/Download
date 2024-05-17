ClearFromBOS()
{
  register int y = curr->w_y, x = curr->w_x;

  LClearArea(&curr->w_layer, 0, 0, x, y, CURR_BCE, 1);
  MClearArea(curr, 0, 0, x, y, CURR_BCE);
  RestorePosRendition();
}
