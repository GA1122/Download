XRRAllocateMonitor(Display *dpy, int noutput)
{
    XRRMonitorInfo *monitor = calloc(1, sizeof (XRRMonitorInfo) + noutput * sizeof (RROutput));
    if (!monitor)
	return NULL;
    monitor->outputs = (RROutput *) (monitor + 1);
    monitor->noutput = noutput;
    return monitor;
}
