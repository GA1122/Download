int runHelper(base::TestSuite* testSuite)
{
    BlinkTestEnvironmentScope blinkTestEnvironment;
    blink::ThreadState::current()->registerTraceDOMWrappers(0, 0);
    int result = testSuite->Run();
    blink::Heap::collectAllGarbage();
    return result;
}
