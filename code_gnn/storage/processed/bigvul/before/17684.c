ProcXFixesDestroyRegion(ClientPtr client)
{
    REQUEST(xXFixesDestroyRegionReq);
    RegionPtr pRegion;

    REQUEST_SIZE_MATCH(xXFixesDestroyRegionReq);
    VERIFY_REGION(pRegion, stuff->region, client, DixWriteAccess);
    FreeResource(stuff->region, RT_NONE);
    return Success;
}
