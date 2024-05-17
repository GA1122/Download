AudioNode* AudioHandler::GetNode() const {
  DCHECK(IsMainThread());
  return node_;
}
