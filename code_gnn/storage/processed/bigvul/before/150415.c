ClientControlledShellSurface::~ClientControlledShellSurface() {
  if (GetWidget())
    GetWindowState()->SetDelegate(nullptr);
  wide_frame_.reset();
  display::Screen::GetScreen()->RemoveObserver(this);
}
