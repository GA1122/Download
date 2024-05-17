ClearToEOS()
{
  register int y = curr->w_y, x = curr->w_x;

  if (x == 0 && y == 0)
    {
      ClearScreen();
      RestorePosRendition();
      return;
    }
  LClearArea(&curr->w_layer, x, y, cols - 1, rows - 1, CURR_BCE, 1);
  MClearArea(curr, x, y, cols - 1, rows - 1, CURR_BCE);
  RestorePosRendition();
}
