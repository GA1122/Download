bool WebViewTest::TapElementById(WebInputEvent::Type type,
                                 const WebString& id) {
  DCHECK(web_view_helper_.GetWebView());
  Element* element = static_cast<Element*>(
      web_view_helper_.LocalMainFrame()->GetDocument().GetElementById(id));
  return TapElement(type, element);
}
