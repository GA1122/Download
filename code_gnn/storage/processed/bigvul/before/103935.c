KeyboardOverlayUI::KeyboardOverlayUI(TabContents* contents)
    : HtmlDialogUI(contents) {
  KeyboardOverlayHandler* handler =
      new KeyboardOverlayHandler(contents->profile());
  AddMessageHandler((handler)->Attach(this));
  KeyboardOverlayUIHTMLSource* html_source = new KeyboardOverlayUIHTMLSource();

  contents->profile()->GetChromeURLDataManager()->AddDataSource(html_source);
}
