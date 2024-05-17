ClearScreen()
{
  LClearArea(&curr->w_layer, 0, 0, curr->w_width - 1, curr->w_height - 1, CURR_BCE, 1);
#ifdef COPY_PASTE
  MScrollV(curr, curr->w_height, 0, curr->w_height - 1, CURR_BCE);
#else
  MClearArea(curr, 0, 0, curr->w_width - 1, curr->w_height - 1, CURR_BCE);
#endif
}
