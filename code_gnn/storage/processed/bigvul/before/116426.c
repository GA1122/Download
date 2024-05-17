void WebTransformOperations::initialize(const WebTransformOperations& other)
{
    if (m_private.get() != other.m_private.get())
        m_private.reset(new WebTransformOperationsPrivate(*other.m_private.get()));
    else
         initialize();
 }
