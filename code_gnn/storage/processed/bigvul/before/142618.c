void TabHelper::RemoveScriptExecutionObserver(
    ScriptExecutionObserver* observer) {
  script_execution_observers_.RemoveObserver(observer);
}
