void TestController::processDidCrash()
{
    if (!m_didPrintWebProcessCrashedMessage) {
#if PLATFORM(MAC)
        pid_t pid = WKPageGetProcessIdentifier(m_mainWebView->page());
        fprintf(stderr, "#CRASHED - WebProcess (pid %ld)\n", static_cast<long>(pid));
#else
        fputs("#CRASHED - WebProcess\n", stderr);
#endif
        fflush(stderr);
        m_didPrintWebProcessCrashedMessage = true;
    }

    if (m_shouldExitWhenWebProcessCrashes)
        exit(1);
}
