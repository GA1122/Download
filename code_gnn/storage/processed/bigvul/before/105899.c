JSValue JSDataView::getFloat32(ExecState* exec)
{
    return getDataViewMember(exec, static_cast<DataView*>(impl()), AccessDataViewMemberAsFloat32);
}
