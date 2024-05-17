void AutofillDialogViews::EraseInvalidViewsInGroup(const DetailsGroup* group) {
  std::map<views::View*, base::string16>::iterator it = validity_map_.begin();
  while (it != validity_map_.end()) {
    if (GroupForView(it->first) == group)
      validity_map_.erase(it++);
    else
      ++it;
  }
}
