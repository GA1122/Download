        virtual ~PendingCommentCallback() 
        {
            xmlFree(s);
        }