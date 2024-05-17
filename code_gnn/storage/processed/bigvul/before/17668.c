SXFixesCursorNotifyEvent(xXFixesCursorNotifyEvent * from,
                         xXFixesCursorNotifyEvent * to)
{
    to->type = from->type;
    cpswaps(from->sequenceNumber, to->sequenceNumber);
    cpswapl(from->window, to->window);
    cpswapl(from->cursorSerial, to->cursorSerial);
    cpswapl(from->timestamp, to->timestamp);
    cpswapl(from->name, to->name);
}
