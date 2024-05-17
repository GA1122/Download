      void doneCurrent()
      {
          if (m_detachedContext)
#if PLATFORM(QT)
            m_detachedContext->makeCurrent(m_detachedSurface);
#elif PLATFORM(EFL)
            glXMakeCurrent(m_display, m_detachedSurface, m_detachedContext);    
#endif
//             glXMakeCurrent(m_display, m_detachedSurface, m_detachedContext);
          m_detachedContext = 0;
      }