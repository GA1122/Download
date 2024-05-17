SnapCoordinator* Document::snapCoordinator()
{
    if (RuntimeEnabledFeatures::cssScrollSnapPointsEnabled() && !m_snapCoordinator)
        m_snapCoordinator = SnapCoordinator::create();

    return m_snapCoordinator.get();
}
