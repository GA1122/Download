void AXTree::UpdateData(const AXTreeData& new_data) {
  if (data_ == new_data)
    return;

  AXTreeData old_data = data_;
  data_ = new_data;
  for (AXTreeObserver& observer : observers_)
    observer.OnTreeDataChanged(this, old_data, new_data);
}
