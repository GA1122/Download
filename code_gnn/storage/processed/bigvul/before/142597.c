void TabHelper::AddScriptExecutionObserver(ScriptExecutionObserver* observer) {
  script_execution_observers_.AddObserver(observer);
}
