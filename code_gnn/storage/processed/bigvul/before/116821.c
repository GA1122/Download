PassRefPtr<PopupContainer> PopupContainer::create(PopupMenuClient* client,
                                                  PopupType popupType,
                                                  const PopupContainerSettings& settings)
{
    return adoptRef(new PopupContainer(client, popupType, settings));
}
