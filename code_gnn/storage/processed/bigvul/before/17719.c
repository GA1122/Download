DGASelectInput(int index, ClientPtr client, long mask)
{
    DGAScreenPtr pScreenPriv = DGA_GET_SCREEN_PRIV(screenInfo.screens[index]);

     
    pScreenPriv->client = client;
    pScreenPriv->input = mask;
}
