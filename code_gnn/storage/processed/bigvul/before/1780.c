    ~Burr()
    {
        m_owner->blockDeleted(this);
        unlink();
    }
