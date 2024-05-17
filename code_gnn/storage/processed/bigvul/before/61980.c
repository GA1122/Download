AcpiDsDoImplicitReturn (
    ACPI_OPERAND_OBJECT     *ReturnDesc,
    ACPI_WALK_STATE         *WalkState,
    BOOLEAN                 AddReference)
{
    ACPI_FUNCTION_NAME (DsDoImplicitReturn);


     
    if ((!AcpiGbl_EnableInterpreterSlack) ||
        (!ReturnDesc))
    {
        return (FALSE);
    }

    ACPI_DEBUG_PRINT ((ACPI_DB_DISPATCH,
        "Result %p will be implicitly returned; Prev=%p\n",
        ReturnDesc,
        WalkState->ImplicitReturnObj));

     
    if (WalkState->ImplicitReturnObj)
    {
        if (WalkState->ImplicitReturnObj == ReturnDesc)
        {
            return (TRUE);
        }
        AcpiDsClearImplicitReturn (WalkState);
    }

     

    WalkState->ImplicitReturnObj = ReturnDesc;
    if (AddReference)
    {
        AcpiUtAddReference (ReturnDesc);
    }

    return (TRUE);
}
