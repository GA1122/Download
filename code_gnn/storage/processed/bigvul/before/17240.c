void SetupAndVerifySignalHandlers() {
  struct sigaction sigact;
  CHECK(sigaction(SIGCHLD, nullptr, &sigact) == 0);
  CHECK(sigact.sa_handler != SIG_IGN) << "SIGCHLD should not be ignored";
  CHECK((sigact.sa_flags & SA_NOCLDWAIT) == 0) <<
      "SA_NOCLDWAIT should not be set";

  CHECK(sigaction(SIGPIPE, nullptr, &sigact) == 0);
  if (sigact.sa_handler == SIG_DFL) {
    sigact.sa_handler = SIG_IGN;
    CHECK(sigaction(SIGPIPE, &sigact, nullptr) == 0);
  }
}
