void ReportInitializationAttemptsSpent(int attempts_spent) {
  UMA_HISTOGRAM_EXACT_LINEAR("OfflinePages.Model.InitAttemptsSpent",
                             attempts_spent, kInitializeAttemptsMax);
}
