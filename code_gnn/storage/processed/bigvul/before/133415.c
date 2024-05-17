bool PaletteDelegateChromeOS::HasNoteApp() {
   return chromeos::IsNoteTakingAppAvailable(GetProfile());
 }
