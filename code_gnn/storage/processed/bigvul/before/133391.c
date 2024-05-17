std::unique_ptr<PaletteDelegate> ShellDelegateImpl::CreatePaletteDelegate() {
  return base::MakeUnique<PaletteDelegateImpl>();
}
