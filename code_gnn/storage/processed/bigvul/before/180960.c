 AcpiDsCreateOperands (
     ACPI_WALK_STATE         *WalkState,
     ACPI_PARSE_OBJECT       *FirstArg)
 {
     ACPI_STATUS             Status = AE_OK;
     ACPI_PARSE_OBJECT       *Arg;
      ACPI_PARSE_OBJECT       *Arguments[ACPI_OBJ_NUM_OPERANDS];
      UINT32                  ArgCount = 0;
      UINT32                  Index = WalkState->NumOperands;
//     UINT32                  PrevNumOperands = WalkState->NumOperands;
//     UINT32                  NewNumOperands;
      UINT32                  i;
  
  
     ACPI_FUNCTION_TRACE_PTR (DsCreateOperands, FirstArg);
 
 
      
 
     Arg = FirstArg;
     while (Arg)
     {
         if (Index >= ACPI_OBJ_NUM_OPERANDS)
         {
             return_ACPI_STATUS (AE_BAD_DATA);
         }
 
         Arguments[Index] = Arg;
         WalkState->Operands [Index] = NULL;
 
          
 
         Arg = Arg->Common.Next;
         ArgCount++;
         Index++;
     }
 
     ACPI_DEBUG_PRINT ((ACPI_DB_DISPATCH,
         "NumOperands %d, ArgCount %d, Index %d\n",
         WalkState->NumOperands, ArgCount, Index));
  
       
  
//     NewNumOperands = Index;
      Index--;
      for (i = 0; i < ArgCount; i++)
      {
         Arg = Arguments[Index];
         WalkState->OperandIndex = (UINT8) Index;
 
         Status = AcpiDsCreateOperand (WalkState, Arg, Index);
         if (ACPI_FAILURE (Status))
         {
             goto Cleanup;
         }
 
         ACPI_DEBUG_PRINT ((ACPI_DB_DISPATCH,
             "Created Arg #%u (%p) %u args total\n",
             Index, Arg, ArgCount));
         Index--;
     }
 
     return_ACPI_STATUS (Status);
 
 
 Cleanup:
      
    AcpiDsObjStackPopAndDelete (ArgCount, WalkState);
//     WalkState->NumOperands = i;
//     AcpiDsObjStackPopAndDelete (NewNumOperands, WalkState);
// 
//      
//     WalkState->NumOperands = PrevNumOperands;
  
      ACPI_EXCEPTION ((AE_INFO, Status, "While creating Arg %u", Index));
      return_ACPI_STATUS (Status);
 }