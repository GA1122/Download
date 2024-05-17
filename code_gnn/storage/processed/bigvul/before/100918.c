void TaskManagerHandler::OnItemsRemoved(const int start, const int length) {
  if (resource_to_group_table_.size() <= static_cast<size_t>(start + length))
    return;

  int group_start = resource_to_group_table_[start];
  int group_end = resource_to_group_table_[start + length - 1];

  if (start != 0 && group_start == resource_to_group_table_[start - 1]) {
    OnGroupChanged(group_start, 1);
    if (group_start == group_end)
      return;
    else
      group_start++;
  }

  if (start + length != model_->ResourceCount() &&
      group_end == resource_to_group_table_[start + length]) {
    OnGroupChanged(group_end, 1);
    if (group_start == group_end)
      return;
    else
      group_end--;
  }

  std::vector<int>::iterator it_first =
      resource_to_group_table_.begin() + start;
  std::vector<int>::iterator it_last = it_first + length - 1;
  resource_to_group_table_.erase(it_first, it_last);

  OnGroupRemoved(group_start, group_end - group_start + 1);
}
