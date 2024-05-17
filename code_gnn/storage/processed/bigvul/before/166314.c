void SpeechRecognitionManagerImpl::AbortSession(int session_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  auto iter = sessions_.find(session_id);
  if (iter == sessions_.end())
    return;

  base::CreateSingleThreadTaskRunnerWithTraits({BrowserThread::UI})
      ->PostTask(
          FROM_HERE,
          base::BindOnce(&SpeechRecognitionManagerImpl::FrameDeletionObserver::
                             RemoveObserverForSession,
                         base::Unretained(frame_deletion_observer_.get()),
                         iter->second->config.initial_context.render_process_id,
                         iter->second->config.initial_context.render_frame_id,
                         session_id));

  AbortSessionImpl(session_id);
}
