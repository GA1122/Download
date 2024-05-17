JSValueRef WebPage::windowObject() const
{
    return toRef(d->m_mainFrame->script()->globalObject(mainThreadNormalWorld()));
}
