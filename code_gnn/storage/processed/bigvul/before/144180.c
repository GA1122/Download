void LockContentsView::GetAccessibleNodeData(ui::AXNodeData* node_data) {
  Shelf* shelf = Shelf::ForWindow(GetWidget()->GetNativeWindow());
  ShelfWidget* shelf_widget = shelf->shelf_widget();
  int next_id = views::AXAuraObjCache::GetInstance()->GetID(shelf_widget);
  node_data->AddIntAttribute(ax::mojom::IntAttribute::kNextFocusId, next_id);

  int previous_id =
      views::AXAuraObjCache::GetInstance()->GetID(shelf->GetStatusAreaWidget());
  node_data->AddIntAttribute(ax::mojom::IntAttribute::kPreviousFocusId,
                             previous_id);
  node_data->SetNameExplicitlyEmpty();
}
