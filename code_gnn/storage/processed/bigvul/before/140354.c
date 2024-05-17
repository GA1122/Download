void Editor::removeFormattingAndStyle() {
  DCHECK(frame().document());
  RemoveFormatCommand::create(*frame().document())->apply();
}
