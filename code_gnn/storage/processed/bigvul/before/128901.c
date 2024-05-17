    StateBase* checkException(StateBase* state)
    {
        return m_tryCatch.HasCaught() ? handleError(JSException, "", state) : 0;
    }
