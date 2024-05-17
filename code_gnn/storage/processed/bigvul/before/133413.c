void PaletteDelegateChromeOS::CreateNote() {
  chromeos::LaunchNoteTakingAppForNewNote(GetProfile(), base::FilePath());
}
