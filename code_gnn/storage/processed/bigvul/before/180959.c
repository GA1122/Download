 AcpiPsCompleteFinalOp (
     ACPI_WALK_STATE         *WalkState,
      ACPI_PARSE_OBJECT       *Op,
      ACPI_STATUS             Status)
  {
    ACPI_STATUS             Status2;
//     ACPI_STATUS             ReturnStatus = AE_OK;
//     BOOLEAN                 Ascending = TRUE;
  
  
      ACPI_FUNCTION_TRACE_PTR (PsCompleteFinalOp, WalkState);
 
 
      
     ACPI_DEBUG_PRINT ((ACPI_DB_PARSE, "AML package complete at Op %p\n", Op));
     do
      {
          if (Op)
          {
            if (WalkState->AscendingCallback != NULL)
//             if (Ascending && WalkState->AscendingCallback != NULL)
              {
                  WalkState->Op = Op;
                  WalkState->OpInfo = AcpiPsGetOpcodeInfo (Op->Common.AmlOpcode);
                 WalkState->Opcode = Op->Common.AmlOpcode;
 
                 Status = WalkState->AscendingCallback (WalkState);
                 Status = AcpiPsNextParseState (WalkState, Op, Status);
                 if (Status == AE_CTRL_PENDING)
                 {
                     Status = AcpiPsCompleteOp (WalkState, &Op, AE_OK);
                     if (ACPI_FAILURE (Status))
                     {
                         return_ACPI_STATUS (Status);
                     }
                 }
  
                  if (Status == AE_CTRL_TERMINATE)
                  {
                    Status = AE_OK;
                     
                    do
                    {
                        if (Op)
                        {
                            Status2 = AcpiPsCompleteThisOp (WalkState, Op);
                            if (ACPI_FAILURE (Status2))
                            {
                                return_ACPI_STATUS (Status2);
                            }
                        }
                        AcpiPsPopScope (&(WalkState->ParserState), &Op,
                            &WalkState->ArgTypes, &WalkState->ArgCount);
                    } while (Op);
                    return_ACPI_STATUS (Status);
//                     Ascending = FALSE;
//                     ReturnStatus = AE_CTRL_TERMINATE;
                  }
  
                  else if (ACPI_FAILURE (Status))
                  {
                       
  
                    (void) AcpiPsCompleteThisOp (WalkState, Op);
                    return_ACPI_STATUS (Status);
//                     Ascending = FALSE;
//                     ReturnStatus = Status;
                  }
              }
  
            Status2 = AcpiPsCompleteThisOp (WalkState, Op);
            if (ACPI_FAILURE (Status2))
//             Status = AcpiPsCompleteThisOp (WalkState, Op);
//             if (ACPI_FAILURE (Status))
              {
                return_ACPI_STATUS (Status2);
//                 Ascending = FALSE;
//                 if (ACPI_SUCCESS (ReturnStatus) ||
//                     ReturnStatus == AE_CTRL_TERMINATE)
//                 {
//                     ReturnStatus = Status;
//                 }
              }
          }
  
         AcpiPsPopScope (&(WalkState->ParserState), &Op, &WalkState->ArgTypes,
             &WalkState->ArgCount);
  
      } while (Op);
  
    return_ACPI_STATUS (Status);
//     return_ACPI_STATUS (ReturnStatus);
  }