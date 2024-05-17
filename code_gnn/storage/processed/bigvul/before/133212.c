void TaskQueue::AsValueInto(base::trace_event::TracedValue* state) const {
  base::AutoLock lock(lock_);
  state->BeginDictionary();
  if (name_)
    state->SetString("name", name_);
  state->SetString("pump_policy",
                   TaskQueueManager::PumpPolicyToString(pump_policy_));
  state->SetString("wakeup_policy",
                   TaskQueueManager::WakeupPolicyToString(wakeup_policy_));
  state->BeginArray("incoming_queue");
  QueueAsValueInto(incoming_queue_, state);
  state->EndArray();
  state->BeginArray("work_queue");
  QueueAsValueInto(work_queue_, state);
  state->EndArray();
  state->BeginArray("delayed_task_queue");
  QueueAsValueInto(delayed_task_queue_, state);
  state->EndArray();
  state->EndDictionary();
}
