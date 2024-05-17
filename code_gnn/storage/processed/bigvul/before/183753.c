     virtual void runTest()
//     void runTest()
      {
        if (m_settings.enableCompositorThread)
            CCLayerTreeHostTest::runTest();
//         CCLayerTreeHostTest::runTest(true);
      }