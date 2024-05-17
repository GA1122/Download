AcpiNsExecModuleCode (
    ACPI_OPERAND_OBJECT     *MethodObj,
    ACPI_EVALUATE_INFO      *Info)
{
    ACPI_OPERAND_OBJECT     *ParentObj;
    ACPI_NAMESPACE_NODE     *ParentNode;
    ACPI_OBJECT_TYPE        Type;
    ACPI_STATUS             Status;


    ACPI_FUNCTION_TRACE (NsExecModuleCode);


     
    ParentNode = ACPI_CAST_PTR (
        ACPI_NAMESPACE_NODE, MethodObj->Method.NextObject);
    Type = AcpiNsGetType (ParentNode);

     
    if ((Type == ACPI_TYPE_DEVICE) && ParentNode->Object)
    {
        MethodObj->Method.Dispatch.Handler =
            ParentNode->Object->Device.Handler;
    }

     

    MethodObj->Method.NextObject = NULL;

     

    memset (Info, 0, sizeof (ACPI_EVALUATE_INFO));
    Info->PrefixNode = ParentNode;

     
    ParentObj = AcpiNsGetAttachedObject (ParentNode);
    if (ParentObj)
    {
        AcpiUtAddReference (ParentObj);
    }

     

    Status = AcpiNsAttachObject (ParentNode, MethodObj, ACPI_TYPE_METHOD);
    if (ACPI_FAILURE (Status))
    {
        goto Exit;
    }

     

    Status = AcpiNsEvaluate (Info);

    ACPI_DEBUG_PRINT ((ACPI_DB_INIT_NAMES,
        "Executed module-level code at %p\n",
        MethodObj->Method.AmlStart));

     

    if (Info->ReturnObject)
    {
        AcpiUtRemoveReference (Info->ReturnObject);
    }

     

    AcpiNsDetachObject (ParentNode);

     

    if (ParentObj)
    {
        Status = AcpiNsAttachObject (ParentNode, ParentObj, Type);
    }
    else
    {
        ParentNode->Type = (UINT8) Type;
    }

Exit:
    if (ParentObj)
    {
        AcpiUtRemoveReference (ParentObj);
    }
    return_VOID;
}
