        virtual ~PendingCharactersCallback() 
        {
            xmlFree(s);
        }