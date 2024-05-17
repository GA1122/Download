Document::EventFactorySet& Document::eventFactories()
{
    DEFINE_STATIC_LOCAL(EventFactorySet, s_eventFactory, ());
    return s_eventFactory;
}
