UpdateCurrentTime(void)
{
    TimeStamp systime;

     
    systime.months = currentTime.months;
    systime.milliseconds = GetTimeInMillis();
    if (systime.milliseconds < currentTime.milliseconds)
        systime.months++;
    if (*checkForInput[0] != *checkForInput[1])
        ProcessInputEvents();
    if (CompareTimeStamps(systime, currentTime) == LATER)
        currentTime = systime;
}
