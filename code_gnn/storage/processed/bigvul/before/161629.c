unsigned AudioHandler::NumberOfOutputChannels() const {
  DCHECK(0) << "numberOfOutputChannels() not valid for node type "
            << GetNodeType();
  return 1;
}
