void HTMLMediaElement::scheduleRejectPlayPromises(ExceptionCode code) {
  DCHECK(m_playPromiseRejectList.isEmpty() ||
         m_playPromiseRejectTaskHandle.isActive());
  if (m_playPromiseResolvers.isEmpty())
    return;

  m_playPromiseRejectList.appendVector(m_playPromiseResolvers);
  m_playPromiseResolvers.clear();

  if (m_playPromiseRejectTaskHandle.isActive())
    return;

  m_playPromiseErrorCode = code;
  m_playPromiseRejectTaskHandle =
      TaskRunnerHelper::get(TaskType::MediaElementEvent, &document())
          ->postCancellableTask(
              BLINK_FROM_HERE,
              WTF::bind(&HTMLMediaElement::rejectScheduledPlayPromises,
                        wrapWeakPersistent(this)));
}
