DevToolsToolboxDelegate::DevToolsToolboxDelegate(
    WebContents* toolbox_contents,
    DevToolsWindow::ObserverWithAccessor* web_contents_observer)
    : WebContentsObserver(toolbox_contents),
      inspected_contents_observer_(web_contents_observer) {
}
