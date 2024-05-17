 AcpiNsEvaluate (
     ACPI_EVALUATE_INFO      *Info)
 {
     ACPI_STATUS             Status;
 
 
     ACPI_FUNCTION_TRACE (NsEvaluate);
 
 
     if (!Info)
     {
         return_ACPI_STATUS (AE_BAD_PARAMETER);
     }
 
     if (!Info->Node)
     {
          
         Status = AcpiNsGetNode (Info->PrefixNode, Info->RelativePathname,
             ACPI_NS_NO_UPSEARCH, &Info->Node);
         if (ACPI_FAILURE (Status))
         {
             return_ACPI_STATUS (Status);
         }
     }
 
      
     if (AcpiNsGetType (Info->Node) == ACPI_TYPE_LOCAL_METHOD_ALIAS)
     {
         Info->Node = ACPI_CAST_PTR (
             ACPI_NAMESPACE_NODE, Info->Node->Object);
     }
 
      
 
     Info->ReturnObject = NULL;
     Info->NodeFlags = Info->Node->Flags;
     Info->ObjDesc = AcpiNsGetAttachedObject (Info->Node);
 
     ACPI_DEBUG_PRINT ((ACPI_DB_NAMES, "%s [%p] Value %p\n",
         Info->RelativePathname, Info->Node,
         AcpiNsGetAttachedObject (Info->Node)));
 
      
 
     Info->Predefined = AcpiUtMatchPredefinedMethod (Info->Node->Name.Ascii);
 
      
 
     Info->FullPathname = AcpiNsGetNormalizedPathname (Info->Node, TRUE);
     if (!Info->FullPathname)
     {
         return_ACPI_STATUS (AE_NO_MEMORY);
     }
 
      
 
     Info->ParamCount = 0;
     if (Info->Parameters)
     {
         while (Info->Parameters[Info->ParamCount])
         {
             Info->ParamCount++;
         }
 
          
 
         if (Info->ParamCount > ACPI_METHOD_NUM_ARGS)
         {
             ACPI_WARN_PREDEFINED ((AE_INFO, Info->FullPathname, ACPI_WARN_ALWAYS,
                 "Excess arguments (%u) - using only %u",
                 Info->ParamCount, ACPI_METHOD_NUM_ARGS));
 
             Info->ParamCount = ACPI_METHOD_NUM_ARGS;
         }
     }
 
      
     AcpiNsCheckAcpiCompliance (Info->FullPathname, Info->Node,
         Info->Predefined);
 
      
     AcpiNsCheckArgumentCount (Info->FullPathname, Info->Node,
         Info->ParamCount, Info->Predefined);
 
      
 
     AcpiNsCheckArgumentTypes (Info);
 
      
     switch (AcpiNsGetType (Info->Node))
     {
     case ACPI_TYPE_DEVICE:
     case ACPI_TYPE_EVENT:
     case ACPI_TYPE_MUTEX:
     case ACPI_TYPE_REGION:
     case ACPI_TYPE_THERMAL:
     case ACPI_TYPE_LOCAL_SCOPE:
          
         ACPI_ERROR ((AE_INFO,
             "%s: Evaluation of object type [%s] is not supported",
             Info->FullPathname,
             AcpiUtGetTypeName (Info->Node->Type)));
 
         Status = AE_TYPE;
         goto Cleanup;
 
     case ACPI_TYPE_METHOD:
          
 
          
 
         if (!Info->ObjDesc)
         {
             ACPI_ERROR ((AE_INFO, "%s: Method has no attached sub-object",
                 Info->FullPathname));
             Status = AE_NULL_OBJECT;
             goto Cleanup;
         }
 
         ACPI_DEBUG_PRINT ((ACPI_DB_EXEC,
             "**** Execute method [%s] at AML address %p length %X\n",
             Info->FullPathname,
             Info->ObjDesc->Method.AmlStart + 1,
             Info->ObjDesc->Method.AmlLength - 1));
 
          
         AcpiExEnterInterpreter ();
         Status = AcpiPsExecuteMethod (Info);
         AcpiExExitInterpreter ();
         break;
 
     default:
          
 
          
         AcpiExEnterInterpreter ();
 
          
 
         Info->ReturnObject = ACPI_CAST_PTR (ACPI_OPERAND_OBJECT, Info->Node);
 
         Status = AcpiExResolveNodeToValue (ACPI_CAST_INDIRECT_PTR (
             ACPI_NAMESPACE_NODE, &Info->ReturnObject), NULL);
         AcpiExExitInterpreter ();
 
         if (ACPI_FAILURE (Status))
         {
             Info->ReturnObject = NULL;
             goto Cleanup;
         }
 
         ACPI_DEBUG_PRINT ((ACPI_DB_NAMES, "Returned object %p [%s]\n",
             Info->ReturnObject,
             AcpiUtGetObjectTypeName (Info->ReturnObject)));
 
         Status = AE_CTRL_RETURN_VALUE;  
         break;
     }
 
      
     (void) AcpiNsCheckReturnValue (Info->Node, Info, Info->ParamCount,
         Status, &Info->ReturnObject);
 
      
 
     if (Status == AE_CTRL_RETURN_VALUE)
     {
          
 
         if (Info->Flags & ACPI_IGNORE_RETURN_VALUE)
         {
             AcpiUtRemoveReference (Info->ReturnObject);
             Info->ReturnObject = NULL;
         }
 
          
  
          Status = AE_OK;
      }
//     else if (ACPI_FAILURE(Status)) 
//     {
//          
// 
//         if (Info->ReturnObject) 
//         {
//             AcpiUtRemoveReference (Info->ReturnObject);
//             Info->ReturnObject = NULL;
//         }
//     }
  
      ACPI_DEBUG_PRINT ((ACPI_DB_NAMES,
          "*** Completed evaluation of object %s ***\n",
         Info->RelativePathname));
 
 Cleanup:
      
     ACPI_FREE (Info->FullPathname);
     Info->FullPathname = NULL;
     return_ACPI_STATUS (Status);
 }