void Instance::Invalidate(uint32 control_id, const pp::Rect& rc) {
  paint_manager_.InvalidateRect(rc);
}
