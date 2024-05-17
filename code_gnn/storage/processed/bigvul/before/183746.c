 void CCThreadProxy::stop()
 {
     TRACE_EVENT("CCThreadProxy::stop", this, 0);
     ASSERT(isMainThread());
     ASSERT(m_started);
 
     CCCompletionEvent completion;
      s_ccThread->postTask(createCCThreadTask(this, &CCThreadProxy::layerTreeHostClosedOnCCThread, AllowCrossThreadAccess(&completion)));
      completion.wait();
  
//     m_mainThreadProxy->shutdown();  
// 
      ASSERT(!m_layerTreeHostImpl);  
      m_layerTreeHost = 0;
      m_started = false;
 }