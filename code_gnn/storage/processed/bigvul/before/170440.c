void Parcel::freeData()
{
    freeDataNoInit();
    initState();
}
