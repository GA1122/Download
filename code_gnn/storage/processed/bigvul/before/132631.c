void BlinkTestRunner::CloseRemainingWindows() {
  NavigateAwayVisitor visitor(render_view());
  RenderView::ForEach(&visitor);
  Send(new ShellViewHostMsg_CloseRemainingWindows(routing_id()));
}
