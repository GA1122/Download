        virtual ~PendingCDATABlockCallback() 
        {
            xmlFree(s);
        }