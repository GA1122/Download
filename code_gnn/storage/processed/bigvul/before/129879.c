void DisableScenarioWhenIdle() {
  BackgroundTracingManager::GetInstance()->SetActiveScenario(
      NULL, BackgroundTracingManager::ReceiveCallback(),
      BackgroundTracingManager::NO_DATA_FILTERING);
}
