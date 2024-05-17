TabContentsContainerGtk::TabContentsContainerGtk(StatusBubbleGtk* status_bubble)
    : tab_(NULL),
      preview_(NULL),
      status_bubble_(status_bubble) {
  Init();
}
