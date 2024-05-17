DbeStubScreen(DbeScreenPrivPtr pDbeScreenPriv, int *nStubbedScreens)
{
     
    pDbeScreenPriv->SetupBackgroundPainter = NULL;

     

     
    pDbeScreenPriv->GetVisualInfo = NULL;
    pDbeScreenPriv->AllocBackBufferName = NULL;
    pDbeScreenPriv->SwapBuffers = NULL;
    pDbeScreenPriv->WinPrivDelete = NULL;

    (*nStubbedScreens)++;

}                                
