bool TestController::runTest(const char* inputLine)
{
    TestCommand command = parseInputLine(std::string(inputLine));

    m_state = RunningTest;

    m_currentInvocation = adoptPtr(new TestInvocation(command.pathOrURL));
    if (command.shouldDumpPixels || m_shouldDumpPixelsForAllTests)
        m_currentInvocation->setIsPixelTest(command.expectedPixelHash);
    if (command.timeout > 0)
        m_currentInvocation->setCustomTimeout(command.timeout);

    m_currentInvocation->invoke();
    m_currentInvocation.clear();

    return true;
}
