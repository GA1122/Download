      void ping()
      {
        CCMainThread::postTask(createMainThreadTask(this, &PingPongTestUsingTasks::pong));
//         m_mainThreadProxy->postTask(createMainThreadTask(this, &PingPongTestUsingTasks::pong));
          hit = true;
      }