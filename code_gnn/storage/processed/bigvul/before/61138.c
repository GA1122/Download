seconds_count_format_time_units (int seconds)
{
    int minutes;
    int hours;

    if (seconds < 0)
    {
         
        seconds = 0;
    }

    if (seconds < 60)
    {
         
        return seconds;
    }

    if (seconds < 60 * 60)
    {
         
        minutes = seconds / 60;
        return minutes;
    }

    hours = seconds / (60 * 60);

    if (seconds < 60 * 60 * 4)
    {
         
        minutes = (seconds - hours * 60 * 60) / 60;
        return minutes + hours;
    }

    return hours;
}
