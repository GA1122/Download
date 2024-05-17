void InjectedBundlePage::didFinishProgress()
{
    if (!InjectedBundle::shared().isTestRunning())
        return;

    if (!InjectedBundle::shared().testRunner()->shouldDumpProgressFinishedCallback())
        return;

    InjectedBundle::shared().outputText("postProgressFinishedNotification\n");
}
