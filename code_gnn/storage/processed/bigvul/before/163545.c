bool ScriptProcessorNode::HasPendingActivity() const {
  if (context()->IsContextClosed())
    return false;

  if (HasEventListeners(EventTypeNames::audioprocess))
    return true;

  return false;
}
