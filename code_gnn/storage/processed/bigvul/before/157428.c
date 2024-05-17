void BaseAudioContext::UpdateWorkletGlobalScopeOnRenderingThread() {
  DCHECK(!IsMainThread());

  if (TryLock()) {
    if (audio_worklet_thread_) {
      AudioWorkletGlobalScope* global_scope =
          ToAudioWorkletGlobalScope(audio_worklet_thread_->GlobalScope());
      DCHECK(global_scope);
      global_scope->SetCurrentFrame(CurrentSampleFrame());
    }

    unlock();
   }
 }