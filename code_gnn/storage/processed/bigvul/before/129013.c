    void writeBooleanObject(bool value)
    {
        append(value ? TrueObjectTag : FalseObjectTag);
    }
