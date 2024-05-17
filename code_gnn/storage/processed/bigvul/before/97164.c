PassRefPtr<Widget> WebFrameLoaderClient::createJavaAppletWidget(
                                           const IntSize& size,
                                           HTMLAppletElement* element,
                                           const KURL&  ,
                                           const Vector<String>& param_names,
                                           const Vector<String>& param_values) {
  return createPlugin(size, element, KURL(), param_names, param_values,
      "application/x-java-applet", false);
}
