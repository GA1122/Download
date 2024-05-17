double currentTime()
{
    wxDateTime now = wxDateTime::UNow();
    return (double)now.GetTicks() + (double)(now.GetMillisecond() / 1000.0);
}
