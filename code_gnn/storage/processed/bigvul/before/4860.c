MonthChangedOrBadTime(CARD32 *ms)
{
     
    if ((currentTime.milliseconds - *ms) > TIMESLOP)
        currentTime.months++;
    else
        *ms = currentTime.milliseconds;
}
