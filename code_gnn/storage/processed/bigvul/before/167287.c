int GetAppIconEndPadding() {
  if (Shell::Get()->tablet_mode_controller()->InTabletMode())
    return 4;
  return 0;
}
