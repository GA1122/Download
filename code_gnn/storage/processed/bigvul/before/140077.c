void HTMLMediaElement::resolveScheduledPlayPromises() {
  for (auto& resolver : m_playPromiseResolveList)
    resolver->resolve();

  m_playPromiseResolveList.clear();
}
