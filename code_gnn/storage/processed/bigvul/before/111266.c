int WebPagePrivate::orientation() const
{
#if ENABLE(ORIENTATION_EVENTS)
    return m_mainFrame->orientation();
#else
#error ORIENTATION_EVENTS must be defined.
#endif
}
