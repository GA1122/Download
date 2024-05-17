CloseDownEvents(void)
{
    FreeEventList(InputEventList, GetMaximumEventsNum());
    InputEventList = NULL;
}
