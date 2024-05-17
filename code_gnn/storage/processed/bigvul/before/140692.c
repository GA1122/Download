base::string16 ScreenLayoutObserverTest::GetFirstDisplayName() {
  return base::UTF8ToUTF16(display_manager()->GetDisplayNameForId(
      display_manager()->first_display_id()));
}
