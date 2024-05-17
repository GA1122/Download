void SetDisplayPropertiesOnHost(AshWindowTreeHost* ash_host,
                                const display::Display& display) {
  DisplayInfo info = GetDisplayManager()->GetDisplayInfo(display.id());
  aura::WindowTreeHost* host = ash_host->AsWindowTreeHost();
#if defined(OS_CHROMEOS)
#if defined(USE_X11)
  const char kRotationProp[] = "_CHROME_DISPLAY_ROTATION";
  const char kScaleFactorProp[] = "_CHROME_DISPLAY_SCALE_FACTOR";
  const char kInternalProp[] = "_CHROME_DISPLAY_INTERNAL";
  const char kCARDINAL[] = "CARDINAL";
  int xrandr_rotation = RR_Rotate_0;
  switch (info.GetActiveRotation()) {
    case display::Display::ROTATE_0:
      xrandr_rotation = RR_Rotate_0;
      break;
    case display::Display::ROTATE_90:
      xrandr_rotation = RR_Rotate_90;
      break;
    case display::Display::ROTATE_180:
      xrandr_rotation = RR_Rotate_180;
      break;
    case display::Display::ROTATE_270:
      xrandr_rotation = RR_Rotate_270;
      break;
  }

  int internal = display.IsInternal() ? 1 : 0;
  gfx::AcceleratedWidget xwindow = host->GetAcceleratedWidget();
  ui::SetIntProperty(xwindow, kInternalProp, kCARDINAL, internal);
  ui::SetIntProperty(xwindow, kRotationProp, kCARDINAL, xrandr_rotation);
  ui::SetIntProperty(xwindow, kScaleFactorProp, kCARDINAL,
                     100 * display.device_scale_factor());
#elif defined(USE_OZONE)
  float scale = display.device_scale_factor();

  if (!display.IsInternal())
    scale *= kCursorMultiplierForExternalDisplays;

  ui::CursorController::GetInstance()->SetCursorConfigForWindow(
      host->GetAcceleratedWidget(), info.GetActiveRotation(), scale);
#endif
#endif
  std::unique_ptr<RootWindowTransformer> transformer(
      CreateRootWindowTransformerForDisplay(host->window(), display));
  ash_host->SetRootWindowTransformer(std::move(transformer));

  scoped_refptr<ManagedDisplayMode> mode =
      GetDisplayManager()->GetActiveModeForDisplayId(display.id());
  if (mode && mode->refresh_rate() > 0.0f) {
    host->compositor()->SetAuthoritativeVSyncInterval(
        base::TimeDelta::FromMicroseconds(base::Time::kMicrosecondsPerSecond /
                                          mode->refresh_rate()));
  }

  host->compositor()->ScheduleFullRedraw();
}
