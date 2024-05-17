base::string16 ScreenLayoutObserverTest::GetMirroringDisplayName() {
  return base::UTF8ToUTF16(display_manager()->GetDisplayNameForId(
      display_manager()->mirroring_display_id()));
}
