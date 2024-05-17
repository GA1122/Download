AcpiDsIsResultUsed (
    ACPI_PARSE_OBJECT       *Op,
    ACPI_WALK_STATE         *WalkState)
{
    const ACPI_OPCODE_INFO  *ParentInfo;

    ACPI_FUNCTION_TRACE_PTR (DsIsResultUsed, Op);


     

    if (!Op)
    {
        ACPI_ERROR ((AE_INFO, "Null Op"));
        return_UINT8 (TRUE);
    }

     
    (void) AcpiDsDoImplicitReturn (WalkState->ResultObj, WalkState, TRUE);

     
    if ((!Op->Common.Parent) ||
        (Op->Common.Parent->Common.AmlOpcode == AML_SCOPE_OP))
    {
         

        ACPI_DEBUG_PRINT ((ACPI_DB_DISPATCH,
            "At Method level, result of [%s] not used\n",
            AcpiPsGetOpcodeName (Op->Common.AmlOpcode)));
        return_UINT8 (FALSE);
    }

     

    ParentInfo = AcpiPsGetOpcodeInfo (Op->Common.Parent->Common.AmlOpcode);
    if (ParentInfo->Class == AML_CLASS_UNKNOWN)
    {
        ACPI_ERROR ((AE_INFO,
            "Unknown parent opcode Op=%p", Op));
        return_UINT8 (FALSE);
    }

     
    switch (ParentInfo->Class)
    {
    case AML_CLASS_CONTROL:

        switch (Op->Common.Parent->Common.AmlOpcode)
        {
        case AML_RETURN_OP:

             

            goto ResultUsed;

        case AML_IF_OP:
        case AML_WHILE_OP:
             
            if ((WalkState->ControlState->Common.State ==
                    ACPI_CONTROL_PREDICATE_EXECUTING) &&
                (WalkState->ControlState->Control.PredicateOp == Op))
            {
                goto ResultUsed;
            }
            break;

        default:

             

            break;
        }

         

        goto ResultNotUsed;

    case AML_CLASS_CREATE:
         
        goto ResultUsed;

    case AML_CLASS_NAMED_OBJECT:

        if ((Op->Common.Parent->Common.AmlOpcode == AML_REGION_OP)       ||
            (Op->Common.Parent->Common.AmlOpcode == AML_DATA_REGION_OP)  ||
            (Op->Common.Parent->Common.AmlOpcode == AML_PACKAGE_OP)      ||
            (Op->Common.Parent->Common.AmlOpcode == AML_BUFFER_OP)       ||
            (Op->Common.Parent->Common.AmlOpcode == AML_VARIABLE_PACKAGE_OP) ||
            (Op->Common.Parent->Common.AmlOpcode == AML_INT_EVAL_SUBTREE_OP) ||
            (Op->Common.Parent->Common.AmlOpcode == AML_BANK_FIELD_OP))
        {
             
            goto ResultUsed;
        }

        goto ResultNotUsed;

    default:
         
        goto ResultUsed;
    }


ResultUsed:
    ACPI_DEBUG_PRINT ((ACPI_DB_DISPATCH,
        "Result of [%s] used by Parent [%s] Op=%p\n",
        AcpiPsGetOpcodeName (Op->Common.AmlOpcode),
        AcpiPsGetOpcodeName (Op->Common.Parent->Common.AmlOpcode), Op));

    return_UINT8 (TRUE);


ResultNotUsed:
    ACPI_DEBUG_PRINT ((ACPI_DB_DISPATCH,
        "Result of [%s] not used by Parent [%s] Op=%p\n",
        AcpiPsGetOpcodeName (Op->Common.AmlOpcode),
        AcpiPsGetOpcodeName (Op->Common.Parent->Common.AmlOpcode), Op));

    return_UINT8 (FALSE);
}
