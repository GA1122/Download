JSValue JSDataView::getUint8(ExecState* exec)
{
    return getDataViewMember(exec, static_cast<DataView*>(impl()), AccessDataViewMemberAsUint8);
}
