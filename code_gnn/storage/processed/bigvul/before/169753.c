_exsltDateDayInWeek(long yday, long yr)
{
    long ret;

    if (yr < 0) {
        ret = ((yr + (((yr+1)/4)-((yr+1)/100)+((yr+1)/400)) + yday) % 7);
        if (ret < 0)
            ret += 7;
    } else
        ret = (((yr-1) + (((yr-1)/4)-((yr-1)/100)+((yr-1)/400)) + yday) % 7);

    return ret;
}
