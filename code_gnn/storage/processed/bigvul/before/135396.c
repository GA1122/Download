bool Document::shouldScheduleLayoutTreeUpdate() const
{
    if (!isActive())
        return false;
    if (inStyleRecalc())
        return false;
    if (m_lifecycle.state() == DocumentLifecycle::InPreLayout)
        return false;
    if (!shouldScheduleLayout())
        return false;
    return true;
}
