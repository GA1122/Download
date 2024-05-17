    StateBase* push(StateBase* state)
    {
        ASSERT(state);
        ++m_depth;
        return checkComposite(state) ? state : handleError(InputError, "Value being cloned is either cyclic or too deeply nested.", state);
    }
