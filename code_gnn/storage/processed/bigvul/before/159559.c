void Document::DidLoadAllScriptBlockingResources() {
  execute_scripts_waiting_for_resources_task_handle_ =
      GetTaskRunner(TaskType::kNetworking)
          ->PostCancellableTask(
              BLINK_FROM_HERE,
              WTF::Bind(&Document::ExecuteScriptsWaitingForResources,
                        WrapWeakPersistent(this)));

  if (IsHTMLDocument() && body()) {
    BeginLifecycleUpdatesIfRenderingReady();
  } else if (!IsHTMLDocument() && documentElement()) {
    BeginLifecycleUpdatesIfRenderingReady();
  }
}
