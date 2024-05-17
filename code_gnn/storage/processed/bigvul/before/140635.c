void OutOfProcessInstance::NotifySelectedFindResultChanged(
    int current_find_index) {
  DCHECK_GE(current_find_index, 0);
  SelectedFindResultChanged(current_find_index);
}
