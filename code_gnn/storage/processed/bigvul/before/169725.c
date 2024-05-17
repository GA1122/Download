ServiceWorkerContainer::ReadyProperty* ServiceWorkerContainer::createReadyProperty()
{
    return new ReadyProperty(getExecutionContext(), this, ReadyProperty::Ready);
}
