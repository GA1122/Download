    static std::unique_ptr<AutofocusTask> create()
    {
        return wrapUnique(new AutofocusTask());
    }
