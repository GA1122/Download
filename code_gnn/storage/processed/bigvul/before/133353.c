void WindowTreeHostManager::DeleteHost(AshWindowTreeHost* host_to_delete) {
  ClearDisplayPropertiesOnHost(host_to_delete);
  RootWindowController* controller =
      GetRootWindowController(GetWindow(host_to_delete));
  DCHECK(controller);
  controller->MoveWindowsTo(GetPrimaryRootWindow());
  controller->Shutdown();
  base::ThreadTaskRunnerHandle::Get()->DeleteSoon(FROM_HERE, controller);
}
