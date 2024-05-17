int HWNDMessageHandler::GetAppbarAutohideEdges(HMONITOR monitor) {
  autohide_factory_.InvalidateWeakPtrs();
  return Appbar::instance()->GetAutohideEdges(
      monitor,
      base::Bind(&HWNDMessageHandler::OnAppbarAutohideEdgesChanged,
                 autohide_factory_.GetWeakPtr()));
}
