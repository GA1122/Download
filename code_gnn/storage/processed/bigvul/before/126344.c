int GetCustomCommandId(GdkEventKey* event) {
  guint modifier = event->state & gtk_accelerator_get_default_mod_mask();
  switch (event->keyval) {
    case GDK_Tab:
    case GDK_ISO_Left_Tab:
    case GDK_KP_Tab:
      if (GDK_CONTROL_MASK == modifier) {
        return IDC_SELECT_NEXT_TAB;
      } else if ((GDK_CONTROL_MASK | GDK_SHIFT_MASK) == modifier) {
        return IDC_SELECT_PREVIOUS_TAB;
      }
      break;

    default:
      break;
  }
  return -1;
}
