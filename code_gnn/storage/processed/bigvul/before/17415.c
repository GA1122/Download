SScreenSaverNotifyEvent(xScreenSaverNotifyEvent * from,
                        xScreenSaverNotifyEvent * to)
{
    to->type = from->type;
    to->state = from->state;
    cpswaps(from->sequenceNumber, to->sequenceNumber);
    cpswapl(from->timestamp, to->timestamp);
    cpswapl(from->root, to->root);
    cpswapl(from->window, to->window);
    to->kind = from->kind;
    to->forced = from->forced;
}
