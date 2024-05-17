 void TouchEventHandler::doFatFingers(Platform::TouchPoint& point)
// void TouchEventHandler::doFatFingers(const Platform::TouchPoint& point)
  {
    m_lastScreenPoint = point.m_screenPos;
//     m_lastScreenPoint = point.screenPosition();
      m_lastFatFingersResult.reset();  
    IntPoint contentPos(m_webPage->mapFromViewportToContents(point.m_pos));
      m_webPage->postponeDocumentStyleRecalc();
    m_lastFatFingersResult = FatFingers(m_webPage, contentPos, FatFingers::ClickableElement).findBestPoint();
//     m_lastFatFingersResult = FatFingers(m_webPage, point.documentContentPosition(), FatFingers::ClickableElement).findBestPoint();
      m_webPage->resumeDocumentStyleRecalc();
  }