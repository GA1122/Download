void TestController::run()
{
    if (!resetStateToConsistentValues()) {
        m_currentInvocation->dumpWebProcessUnresponsiveness();
        return;
    }

    if (m_usingServerMode)
        runTestingServerLoop();
    else {
        for (size_t i = 0; i < m_paths.size(); ++i) {
            if (!runTest(m_paths[i].c_str()))
                break;
        }
    }
}
