void TaskManagerHandler::OnItemsChanged(const int start, const int length) {
  UpdateResourceGroupTable(start, length);

  int group_start = model_->GetGroupIndexForResource(start);
  int group_end = model_->GetGroupIndexForResource(start + length - 1);

  OnGroupChanged(group_start, group_end - group_start + 1);
}
