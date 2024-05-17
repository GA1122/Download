JSValue JSDataView::getInt8(ExecState* exec)
{
    return getDataViewMember(exec, static_cast<DataView*>(impl()), AccessDataViewMemberAsInt8);
}
