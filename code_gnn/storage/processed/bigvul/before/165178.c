static DataTransfer* CreateDraggingDataTransfer(DataTransferAccessPolicy policy,
                                                DragData* drag_data) {
  return DataTransfer::Create(DataTransfer::kDragAndDrop, policy,
                              drag_data->PlatformData());
}
