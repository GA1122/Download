void ClearDisplayPropertiesOnHost(AshWindowTreeHost* ash_host) {
#if defined(OS_CHROMEOS) && defined(USE_OZONE)
  aura::WindowTreeHost* host = ash_host->AsWindowTreeHost();
  ui::CursorController::GetInstance()->ClearCursorConfigForWindow(
      host->GetAcceleratedWidget());
#endif
}
