  static WebString GetValueWrapper(WebFormControlElement element) {
    if (element.FormControlType() == "textarea")
      return element.To<WebFormControlElement>().Value();

    if (element.FormControlType() == "select-one")
      return element.To<WebSelectElement>().Value();

    return element.To<WebInputElement>().Value();
  }
