void Editor::RegisterCommandGroup(CompositeEditCommand* command_group_wrapper) {
  DCHECK(command_group_wrapper->IsCommandGroupWrapper());
  last_edit_command_ = command_group_wrapper;
}
