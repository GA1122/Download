void OutOfProcessInstance::UpdateCursor(PP_CursorType_Dev cursor) {
  if (cursor == cursor_)
    return;
  cursor_ = cursor;

  const PPB_CursorControl_Dev* cursor_interface =
      reinterpret_cast<const PPB_CursorControl_Dev*>(
      pp::Module::Get()->GetBrowserInterface(PPB_CURSOR_CONTROL_DEV_INTERFACE));
  if (!cursor_interface) {
    NOTREACHED();
    return;
  }

  cursor_interface->SetCursor(
      pp_instance(), cursor_, pp::ImageData().pp_resource(), nullptr);
}
