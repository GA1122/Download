void PrintWebViewHelper::OnPrintPages() {
  WebKit::WebFrame* frame;
  if (GetPrintFrame(&frame))
    Print(frame, WebKit::WebNode());
}
