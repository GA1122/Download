JSValue JSDataView::setInt8(ExecState* exec)
{
    return setDataViewMember(exec, static_cast<DataView*>(impl()), AccessDataViewMemberAsInt8);
}
