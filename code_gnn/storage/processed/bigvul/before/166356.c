void SpeechRecognitionManagerImpl::StopAudioCaptureForSession(int session_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  auto iter = sessions_.find(session_id);
  if (iter == sessions_.end())
    return;

  iter->second->ui.reset();

  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::BindOnce(&SpeechRecognitionManagerImpl::DispatchEvent,
                                weak_factory_.GetWeakPtr(), session_id,
                                EVENT_STOP_CAPTURE));
}
