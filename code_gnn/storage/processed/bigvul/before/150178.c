bool HasPendingUncleanExit(Profile* profile) {
  return profile->GetLastSessionExitType() == Profile::EXIT_CRASHED &&
      !profile_launch_observer.Get().HasBeenLaunched(profile);
}
