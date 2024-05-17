void TaskManagerView::ViewHierarchyChanged(bool is_add,
                                           views::View* parent,
                                           views::View* child) {
  if (child == this) {
    if (is_add) {
      parent->AddChildView(about_memory_link_);
      if (purge_memory_button_)
        parent->AddChildView(purge_memory_button_);
      parent->AddChildView(kill_button_);
      AddChildView(tab_table_);
    } else {
      parent->RemoveChildView(kill_button_);
      if (purge_memory_button_)
        parent->RemoveChildView(purge_memory_button_);
      parent->RemoveChildView(about_memory_link_);
    }
  }
}
