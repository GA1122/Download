DEFINE_TRACE(HTMLFormElement) {
  visitor->trace(m_pastNamesMap);
  visitor->trace(m_radioButtonGroupScope);
  visitor->trace(m_associatedElements);
  visitor->trace(m_imageElements);
  visitor->trace(m_plannedNavigation);
  HTMLElement::trace(visitor);
}
