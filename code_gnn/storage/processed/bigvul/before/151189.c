InspectorNetworkAgent::InspectorNetworkAgent(
    InspectedFrames* inspected_frames,
    WorkerGlobalScope* worker_global_scope)
    : inspected_frames_(inspected_frames),
      worker_global_scope_(worker_global_scope),
      resources_data_(
          NetworkResourcesData::Create(g_maximum_total_buffer_size,
                                       g_maximum_resource_buffer_size)),
      pending_request_(nullptr),
      remove_finished_replay_xhr_timer_(
          inspected_frames ? TaskRunnerHelper::Get(TaskType::kUnspecedLoading,
                                                   inspected_frames->Root())
                           : TaskRunnerHelper::Get(TaskType::kUnspecedLoading,
                                                   worker_global_scope),
          this,
          &InspectorNetworkAgent::RemoveFinishedReplayXHRFired) {
  DCHECK((IsMainThread() && inspected_frames_ && !worker_global_scope_) ||
         (!IsMainThread() && !inspected_frames_ && worker_global_scope_));
}
