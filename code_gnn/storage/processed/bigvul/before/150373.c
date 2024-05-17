void ClientControlledShellSurface::OnDeviceScaleFactorChanged(float old_dsf,
                                                              float new_dsf) {
  views::View::OnDeviceScaleFactorChanged(old_dsf, new_dsf);
  UpdateFrameWidth();
}
