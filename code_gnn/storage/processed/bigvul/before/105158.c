String Range::text() const
{
    if (!m_start.container())
        return String();

    m_start.container()->document()->updateLayout();

     return plainText(this);
 }
