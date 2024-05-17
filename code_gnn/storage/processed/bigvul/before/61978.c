AcpiDsCreateOperand (
    ACPI_WALK_STATE         *WalkState,
    ACPI_PARSE_OBJECT       *Arg,
    UINT32                  ArgIndex)
{
    ACPI_STATUS             Status = AE_OK;
    char                    *NameString;
    UINT32                  NameLength;
    ACPI_OPERAND_OBJECT     *ObjDesc;
    ACPI_PARSE_OBJECT       *ParentOp;
    UINT16                  Opcode;
    ACPI_INTERPRETER_MODE   InterpreterMode;
    const ACPI_OPCODE_INFO  *OpInfo;


    ACPI_FUNCTION_TRACE_PTR (DsCreateOperand, Arg);


     

    if ((Arg->Common.AmlOpcode == AML_INT_NAMEPATH_OP) &&
        (Arg->Common.Value.String) &&
        !(Arg->Common.Flags & ACPI_PARSEOP_IN_STACK))
    {
        ACPI_DEBUG_PRINT ((ACPI_DB_DISPATCH, "Getting a name: Arg=%p\n", Arg));

         

        Status = AcpiExGetNameString (ACPI_TYPE_ANY,
            Arg->Common.Value.Buffer, &NameString, &NameLength);

        if (ACPI_FAILURE (Status))
        {
            return_ACPI_STATUS (Status);
        }

         

         
        if ((WalkState->DeferredNode) &&
            (WalkState->DeferredNode->Type == ACPI_TYPE_BUFFER_FIELD) &&
            (ArgIndex == (UINT32)
                ((WalkState->Opcode == AML_CREATE_FIELD_OP) ? 3 : 2)))
        {
            ObjDesc = ACPI_CAST_PTR (
                ACPI_OPERAND_OBJECT, WalkState->DeferredNode);
            Status = AE_OK;
        }
        else     
        {
             
            ParentOp = Arg->Common.Parent;
            OpInfo = AcpiPsGetOpcodeInfo (ParentOp->Common.AmlOpcode);

            if ((OpInfo->Flags & AML_NSNODE) &&
                (ParentOp->Common.AmlOpcode != AML_INT_METHODCALL_OP) &&
                (ParentOp->Common.AmlOpcode != AML_REGION_OP) &&
                (ParentOp->Common.AmlOpcode != AML_INT_NAMEPATH_OP))
            {
                 

                InterpreterMode = ACPI_IMODE_LOAD_PASS2;
            }
            else
            {
                 

                InterpreterMode = ACPI_IMODE_EXECUTE;
            }

            Status = AcpiNsLookup (WalkState->ScopeInfo, NameString,
                ACPI_TYPE_ANY, InterpreterMode,
                ACPI_NS_SEARCH_PARENT | ACPI_NS_DONT_OPEN_SCOPE, WalkState,
                ACPI_CAST_INDIRECT_PTR (ACPI_NAMESPACE_NODE, &ObjDesc));
             
            if (Status == AE_NOT_FOUND)
            {
                if (ParentOp->Common.AmlOpcode == AML_CONDITIONAL_REF_OF_OP)
                {
                     
                    ObjDesc = ACPI_CAST_PTR (
                        ACPI_OPERAND_OBJECT, AcpiGbl_RootNode);
                    Status = AE_OK;
                }
                else if (ParentOp->Common.AmlOpcode == AML_EXTERNAL_OP)
                {
                     
                    Status = AE_AML_BAD_OPCODE;
                }
                else
                {
                     
                    Status = AE_AML_NAME_NOT_FOUND;
                }
            }

            if (ACPI_FAILURE (Status))
            {
                ACPI_ERROR_NAMESPACE (NameString, Status);
            }
        }

         

        ACPI_FREE (NameString);

         

        if (ACPI_FAILURE (Status))
        {
            return_ACPI_STATUS (Status);
        }

         

        Status = AcpiDsObjStackPush (ObjDesc, WalkState);
        if (ACPI_FAILURE (Status))
        {
            return_ACPI_STATUS (Status);
        }

        AcpiDbDisplayArgumentObject (ObjDesc, WalkState);
    }
    else
    {
         

        if ((Arg->Common.AmlOpcode == AML_INT_NAMEPATH_OP) &&
            !(Arg->Common.Flags & ACPI_PARSEOP_IN_STACK))
        {
             
            Opcode = AML_ZERO_OP;        

            ACPI_DEBUG_PRINT ((ACPI_DB_DISPATCH,
                "Null namepath: Arg=%p\n", Arg));
        }
        else
        {
            Opcode = Arg->Common.AmlOpcode;
        }

         

        OpInfo = AcpiPsGetOpcodeInfo (Opcode);
        if (OpInfo->ObjectType == ACPI_TYPE_INVALID)
        {
            return_ACPI_STATUS (AE_NOT_IMPLEMENTED);
        }

        if ((OpInfo->Flags & AML_HAS_RETVAL) ||
            (Arg->Common.Flags & ACPI_PARSEOP_IN_STACK))
        {
             
            Status = AcpiDsResultPop (&ObjDesc, WalkState);
            if (ACPI_FAILURE (Status))
            {
                 
                ACPI_EXCEPTION ((AE_INFO, Status,
                    "Missing or null operand"));
                return_ACPI_STATUS (Status);
            }
        }
        else
        {
             

            ObjDesc = AcpiUtCreateInternalObject (OpInfo->ObjectType);
            if (!ObjDesc)
            {
                return_ACPI_STATUS (AE_NO_MEMORY);
            }

             

            Status = AcpiDsInitObjectFromOp (
                WalkState, Arg, Opcode, &ObjDesc);
            if (ACPI_FAILURE (Status))
            {
                AcpiUtDeleteObjectDesc (ObjDesc);
                return_ACPI_STATUS (Status);
            }
        }

         

        Status = AcpiDsObjStackPush (ObjDesc, WalkState);
        if (ACPI_FAILURE (Status))
        {
            return_ACPI_STATUS (Status);
        }

        AcpiDbDisplayArgumentObject (ObjDesc, WalkState);
    }

    return_ACPI_STATUS (AE_OK);
}
