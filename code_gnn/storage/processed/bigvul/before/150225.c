void UpdateDeskContainersBackdrops() {
  for (auto* root : Shell::GetAllRootWindows()) {
    for (auto* desk_container : desks_util::GetDesksContainers(root)) {
      WorkspaceController* controller = GetWorkspaceController(desk_container);
      WorkspaceLayoutManager* layout_manager = controller->layout_manager();
      BackdropController* backdrop_controller =
          layout_manager->backdrop_controller();
      backdrop_controller->UpdateBackdrop();
    }
  }
}
