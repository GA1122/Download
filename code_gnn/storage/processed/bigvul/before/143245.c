void Document::registerEventFactory(std::unique_ptr<EventFactoryBase> eventFactory)
{
    DCHECK(!eventFactories().contains(eventFactory.get()));
    eventFactories().add(std::move(eventFactory));
}
