TestController::TestController(int argc, const char* argv[])
    : m_verbose(false)
    , m_printSeparators(false)
    , m_usingServerMode(false)
    , m_gcBetweenTests(false)
    , m_shouldDumpPixelsForAllTests(false)
    , m_state(Initial)
    , m_doneResetting(false)
    , m_longTimeout(defaultLongTimeout)
    , m_shortTimeout(defaultShortTimeout)
    , m_noTimeout(defaultNoTimeout)
    , m_useWaitToDumpWatchdogTimer(true)
    , m_forceNoTimeout(false)
    , m_timeout(0)
    , m_didPrintWebProcessCrashedMessage(false)
    , m_shouldExitWhenWebProcessCrashes(true)
    , m_beforeUnloadReturnValue(true)
    , m_isGeolocationPermissionSet(false)
    , m_isGeolocationPermissionAllowed(false)
    , m_policyDelegateEnabled(false)
    , m_policyDelegatePermissive(false)
{
    initialize(argc, argv);
    controller = this;
    run();
    controller = 0;
}
