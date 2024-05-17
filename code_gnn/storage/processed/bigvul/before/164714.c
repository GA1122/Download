bool ClickElement(const WebDocument& document,
                  const WebElementDescriptor& element_descriptor) {
  WebString web_descriptor = WebString::FromUTF8(element_descriptor.descriptor);
  blink::WebElement element;

  switch (element_descriptor.retrieval_method) {
    case WebElementDescriptor::CSS_SELECTOR: {
      element = document.QuerySelector(web_descriptor);
      break;
    }
    case WebElementDescriptor::ID:
      element = document.GetElementById(web_descriptor);
      break;
    case WebElementDescriptor::NONE:
      return true;
  }

  if (element.IsNull()) {
    DVLOG(1) << "Could not find "
             << element_descriptor.descriptor
             << " by "
             << RetrievalMethodToString(element_descriptor.retrieval_method)
             << ".";
    return false;
  }

  element.SimulateClick();
  return true;
}
