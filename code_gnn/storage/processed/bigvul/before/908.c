void CairoOutputDev::popTransparencyGroup() {
   
  ColorSpaceStack *css = groupColorSpaceStack;
  if (css->knockout) {
    knockoutCount--;
    if (!knockoutCount) {
       
      cairo_destroy(cairo_shape);
      cairo_shape = NULL;
    }
  }
  groupColorSpaceStack = css->next;
  delete css;
}
