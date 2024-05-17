Editor::Command::Command(const EditorInternalCommand* command,
                         EditorCommandSource source,
                         LocalFrame* frame)
    : command_(command), source_(source), frame_(command ? frame : nullptr) {
  if (!command)
    DCHECK(!frame_);
  else
    DCHECK(frame_);
}
