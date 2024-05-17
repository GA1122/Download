TabHandler* TabHandler::CreateTabHandler(TabHandlerDelegate* delegate) {
  return new DefaultTabHandler(delegate);
}
