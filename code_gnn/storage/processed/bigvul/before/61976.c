AcpiDsClearImplicitReturn (
    ACPI_WALK_STATE         *WalkState)
{
    ACPI_FUNCTION_NAME (DsClearImplicitReturn);


     
    if (!AcpiGbl_EnableInterpreterSlack)
    {
        return;
    }

    if (WalkState->ImplicitReturnObj)
    {
         
        ACPI_DEBUG_PRINT ((ACPI_DB_DISPATCH,
            "Removing reference on stale implicit return obj %p\n",
            WalkState->ImplicitReturnObj));

        AcpiUtRemoveReference (WalkState->ImplicitReturnObj);
        WalkState->ImplicitReturnObj = NULL;
    }
}
