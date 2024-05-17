  PlatformTouchPoint::PlatformTouchPoint(const BlackBerry::Platform::TouchPoint& point)
    : m_id(point.m_id)
    , m_screenPos(point.m_screenPos)
    , m_pos(point.m_pos)
//     : m_id(point.id())
//     , m_screenPos(point.screenPosition())
//      
//      
//      
//     , m_pos(point.documentViewportPosition())
  {
    switch (point.m_state) {
//     switch (point.state()) {
      case BlackBerry::Platform::TouchPoint::TouchReleased:
          m_state = TouchReleased;
          break;
     case BlackBerry::Platform::TouchPoint::TouchMoved:
         m_state = TouchMoved;
         break;
     case BlackBerry::Platform::TouchPoint::TouchPressed:
         m_state = TouchPressed;
         break;
     case BlackBerry::Platform::TouchPoint::TouchStationary:
         m_state = TouchStationary;
         break;
     default:
         m_state = TouchStationary;  
         BLACKBERRY_ASSERT(false);
         break;
     }
 }