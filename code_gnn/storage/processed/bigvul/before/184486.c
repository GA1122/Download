 void WebPagePrivate::willComposite()
  {
      if (!m_page->settings()->developerExtrasEnabled())
          return;
    InspectorInstrumentation::willComposite(m_page);
//     m_page->inspectorController()->willComposite();
  }