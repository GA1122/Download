String GetTypeExtension(Document* document,
                        const StringOrDictionary& string_or_options,
                        ExceptionState& exception_state) {
  if (string_or_options.isNull())
    return g_empty_string;

  if (string_or_options.isString()) {
    UseCounter::Count(document,
                      WebFeature::kDocumentCreateElement2ndArgStringHandling);
    return string_or_options.getAsString();
  }

  if (string_or_options.isDictionary()) {
    Dictionary dict = string_or_options.getAsDictionary();
    ElementCreationOptions impl;
    V8ElementCreationOptions::toImpl(dict.GetIsolate(), dict.V8Value(), impl,
                                     exception_state);
    if (exception_state.HadException())
      return g_empty_string;

    if (impl.hasIs())
      return impl.is();
  }

  return g_empty_string;
}
