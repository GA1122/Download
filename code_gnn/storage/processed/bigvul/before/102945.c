void TabStripModel::AddTabContents(TabContentsWrapper* contents,
                                   int index,
                                   PageTransition::Type transition,
                                   int add_types) {
  bool inherit_group = (add_types & ADD_INHERIT_GROUP) == ADD_INHERIT_GROUP;

  if (transition == PageTransition::LINK &&
      (add_types & ADD_FORCE_INDEX) == 0) {
    index = order_controller_->DetermineInsertionIndex(
        contents, transition, add_types & ADD_ACTIVE);
    inherit_group = true;
  } else {
    if (index < 0 || index > count())
      index = order_controller_->DetermineInsertionIndexForAppending();
  }

  if (transition == PageTransition::TYPED && index == count()) {
    inherit_group = true;
  }
  InsertTabContentsAt(
      index, contents,
      add_types | (inherit_group ? ADD_INHERIT_GROUP : 0));
  index = GetIndexOfTabContents(contents);

  if (inherit_group && transition == PageTransition::TYPED)
    contents_data_.at(index)->reset_group_on_select = true;

  if (TabContentsWrapper* old_contents = GetSelectedTabContents()) {
    if ((add_types & ADD_ACTIVE) == 0) {
      contents->tab_contents()->view()->
          SizeContents(old_contents->tab_contents()->
                          view()->GetContainerSize());
      contents->tab_contents()->HideContents();
    }
  }
}
