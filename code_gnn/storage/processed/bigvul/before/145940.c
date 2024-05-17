  void DragMove(int dx, int dy) {
    resizer_->Drag(CalculateDragPoint(*resizer_, dx, dy), 0);
  }
