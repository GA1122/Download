    void fakeDraw()
    {
        if (m_isDirty)
            return;
        m_isDirty = true;
        Platform::current()->currentThread()->addTaskObserver(this);
    }
