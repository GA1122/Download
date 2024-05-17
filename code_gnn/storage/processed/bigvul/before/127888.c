        explicit EmptyDataReader(WebDataConsumerHandle::Client* client) : m_factory(this)
        {
            Platform::current()->currentThread()->getWebTaskRunner()->postTask(BLINK_FROM_HERE, bind(&EmptyDataReader::notify, m_factory.createWeakPtr(), client));
        }
