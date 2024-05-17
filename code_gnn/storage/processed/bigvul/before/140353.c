void Editor::registerCommandGroup(CompositeEditCommand* commandGroupWrapper) {
  DCHECK(commandGroupWrapper->isCommandGroupWrapper());
  m_lastEditCommand = commandGroupWrapper;
}
