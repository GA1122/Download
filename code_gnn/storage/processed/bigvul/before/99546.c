CustomDrag::CustomDrag(SkBitmap* icon, int code_mask, GdkDragAction action)
    : drag_widget_(gtk_invisible_new()),
      pixbuf_(icon ? gfx::GdkPixbufFromSkBitmap(icon) : NULL) {
  g_object_ref_sink(drag_widget_);
  g_signal_connect(drag_widget_, "drag-data-get",
                   G_CALLBACK(OnDragDataGetThunk), this);
  g_signal_connect(drag_widget_, "drag-begin",
                   G_CALLBACK(OnDragBeginThunk), this);
  g_signal_connect(drag_widget_, "drag-end",
                   G_CALLBACK(OnDragEndThunk), this);

  GtkTargetList* list = gtk_dnd_util::GetTargetListFromCodeMask(code_mask);
  GdkEvent* event = gtk_get_current_event();
  gtk_drag_begin(drag_widget_, list, action, 1, event);
  if (event)
    gdk_event_free(event);
  gtk_target_list_unref(list);
}
