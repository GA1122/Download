base::string16 ScreenLayoutObserverTest::GetSecondDisplayName() {
  return base::UTF8ToUTF16(display_manager()->GetDisplayNameForId(
      display_manager()->GetSecondaryDisplay().id()));
}
