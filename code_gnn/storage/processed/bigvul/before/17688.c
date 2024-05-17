PanoramiXFixesReset(void)
{
    int i;

    for (i = 0; i < XFixesNumberRequests; i++)
        ProcXFixesVector[i] = PanoramiXSaveXFixesVector[i];
}
