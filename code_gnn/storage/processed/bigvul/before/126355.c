int GetPreHandleCommandId(GdkEventKey* event) {
  guint modifier = event->state & gtk_accelerator_get_default_mod_mask();
  switch (event->keyval) {
    case GDK_Page_Down:
      if (GDK_CONTROL_MASK == modifier) {
        return IDC_SELECT_NEXT_TAB;
      } else if ((GDK_CONTROL_MASK | GDK_SHIFT_MASK) == modifier) {
        return IDC_MOVE_TAB_NEXT;
      }
      break;

    case GDK_Page_Up:
      if (GDK_CONTROL_MASK == modifier) {
        return IDC_SELECT_PREVIOUS_TAB;
      } else if ((GDK_CONTROL_MASK | GDK_SHIFT_MASK) == modifier) {
        return IDC_MOVE_TAB_PREVIOUS;
      }
      break;

    default:
      break;
  }
  return -1;
}
