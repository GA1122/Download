void DocumentLoader::setMainResourceDataBufferingPolicy(DataBufferingPolicy dataBufferingPolicy)
{
    if (m_mainResource)
        m_mainResource->setDataBufferingPolicy(dataBufferingPolicy);
}
