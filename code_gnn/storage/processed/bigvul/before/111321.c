JSContextRef WebPage::scriptContext() const
{
    if (!d->m_mainFrame)
        return 0;

    JSC::Bindings::RootObject *root = d->m_mainFrame->script()->bindingRootObject();
    if (!root)
        return 0;

    JSC::ExecState *exec = root->globalObject()->globalExec();
    return toRef(exec);
}
