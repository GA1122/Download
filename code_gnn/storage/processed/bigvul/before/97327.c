        virtual ~PendingErrorCallback() 
        {
            free (message);
        }