  static WebString GetSuggestedValueWrapper(WebFormControlElement element) {
    if (element.FormControlType() == "textarea")
      return element.To<WebFormControlElement>().SuggestedValue();

    if (element.FormControlType() == "select-one")
      return element.To<WebSelectElement>().SuggestedValue();

    return element.To<WebInputElement>().SuggestedValue();
  }
