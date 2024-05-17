void HandleToggleMirrorMode() {
  base::RecordAction(UserMetricsAction("Accel_Toggle_Mirror_Mode"));
  bool mirror = !Shell::GetInstance()->display_manager()->IsInMirrorMode();
  Shell::GetInstance()->display_configuration_controller()->SetMirrorMode(
      mirror, true  );
}
