bool AutofillDialogViews::ValidateForm() {
  bool all_valid = true;
  validity_map_.clear();

  for (DetailGroupMap::iterator iter = detail_groups_.begin();
       iter != detail_groups_.end(); ++iter) {
    const DetailsGroup& group = iter->second;
    if (!group.container->visible())
      continue;

    if (!ValidateGroup(group, VALIDATE_FINAL))
      all_valid = false;
  }

  return all_valid;
}
