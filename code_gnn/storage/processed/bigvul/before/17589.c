ProcRenderFreePicture(ClientPtr client)
{
    PicturePtr pPicture;

    REQUEST(xRenderFreePictureReq);

    REQUEST_SIZE_MATCH(xRenderFreePictureReq);

    VERIFY_PICTURE(pPicture, stuff->picture, client, DixDestroyAccess);
    FreeResource(stuff->picture, RT_NONE);
    return Success;
}
