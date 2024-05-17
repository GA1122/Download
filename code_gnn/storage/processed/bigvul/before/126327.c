void BrowserWindowGtk::DrawCustomFrameBorder(GtkWidget* widget) {
  static NineBox* custom_frame_border = NULL;
  if (!custom_frame_border) {
    custom_frame_border = new NineBox(IDR_WINDOW_TOP_LEFT_CORNER,
                                      IDR_WINDOW_TOP_CENTER,
                                      IDR_WINDOW_TOP_RIGHT_CORNER,
                                      IDR_WINDOW_LEFT_SIDE,
                                      0,
                                      IDR_WINDOW_RIGHT_SIDE,
                                      IDR_WINDOW_BOTTOM_LEFT_CORNER,
                                      IDR_WINDOW_BOTTOM_CENTER,
                                      IDR_WINDOW_BOTTOM_RIGHT_CORNER);
  }
  custom_frame_border->RenderToWidget(widget);
}
