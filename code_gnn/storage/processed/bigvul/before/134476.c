int HomeButton::OnDragUpdated(const ui::DropTargetEvent& event) {
  return (event.source_operations() & ui::DragDropTypes::DRAG_LINK) ?
      ui::DragDropTypes::DRAG_LINK : ui::DragDropTypes::DRAG_NONE;
}
