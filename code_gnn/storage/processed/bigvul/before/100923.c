void TaskManagerHandler::UpdateResourceGroupTable(int start, int length) {
  if (resource_to_group_table_.size() < static_cast<size_t>(start)) {
    length += start - resource_to_group_table_.size();
    start = resource_to_group_table_.size();
  }

  std::vector<int>::iterator it = resource_to_group_table_.begin() + start;
  resource_to_group_table_.insert(it, static_cast<size_t>(length), -1);

  for (int i = start; i < start + length; i++) {
    const int group_index = model_->GetGroupIndexForResource(i);
    resource_to_group_table_[i] = group_index;
  }
}
