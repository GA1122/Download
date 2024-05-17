void Document::DidLoadAllScriptBlockingResources() {
  execute_scripts_waiting_for_resources_task_handle_ =
      TaskRunnerHelper::Get(TaskType::kNetworking, this)
          ->PostCancellableTask(
              BLINK_FROM_HERE,
              WTF::Bind(&Document::ExecuteScriptsWaitingForResources,
                        WrapWeakPersistent(this)));

  if (IsHTMLDocument() && body()) {
    BeginLifecycleUpdatesIfRenderingReady();
  } else if (!IsHTMLDocument() && documentElement()) {
    BeginLifecycleUpdatesIfRenderingReady();
  }

  if (goto_anchor_needed_after_stylesheets_load_ && View())
    View()->ProcessUrlFragment(url_);
}
