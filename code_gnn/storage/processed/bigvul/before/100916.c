void TaskManagerHandler::OnItemsAdded(const int start, const int length) {
  UpdateResourceGroupTable(start, length);

  int group_start = model_->GetGroupIndexForResource(start);
  int group_end = model_->GetGroupIndexForResource(start + length - 1);

  if (!model_->IsResourceFirstInGroup(start)) {
    OnGroupChanged(group_start, 1);
    if (group_start == group_end)
      return;
    else
      group_start++;
  }

  if (!model_->IsResourceLastInGroup(start + length - 1)) {
    OnGroupChanged(group_end, 1);
    if (group_start == group_end)
      return;
    else
      group_end--;
  }

  OnGroupAdded(group_start, group_end - group_start + 1);
}
