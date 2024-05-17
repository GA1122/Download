WebNotificationData::Direction toDirectionEnumValue(const String& direction)
{
    if (direction == "ltr")
        return WebNotificationData::DirectionLeftToRight;
    if (direction == "rtl")
        return WebNotificationData::DirectionRightToLeft;

    return WebNotificationData::DirectionAuto;
}
