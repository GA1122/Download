AcpiPsGetAmlOpcode (
    ACPI_WALK_STATE         *WalkState)
{
    UINT32                  AmlOffset;


    ACPI_FUNCTION_TRACE_PTR (PsGetAmlOpcode, WalkState);


    WalkState->Aml = WalkState->ParserState.Aml;
    WalkState->Opcode = AcpiPsPeekOpcode (&(WalkState->ParserState));

     
    WalkState->OpInfo = AcpiPsGetOpcodeInfo (WalkState->Opcode);

    switch (WalkState->OpInfo->Class)
    {
    case AML_CLASS_ASCII:
    case AML_CLASS_PREFIX:
         
        WalkState->Opcode = AML_INT_NAMEPATH_OP;
        WalkState->ArgTypes = ARGP_NAMESTRING;
        break;

    case AML_CLASS_UNKNOWN:

         

        if (WalkState->PassNumber == 2)
        {
            AmlOffset = (UINT32) ACPI_PTR_DIFF (WalkState->Aml,
                WalkState->ParserState.AmlStart);

            ACPI_ERROR ((AE_INFO,
                "Unknown opcode 0x%.2X at table offset 0x%.4X, ignoring",
                WalkState->Opcode,
                (UINT32) (AmlOffset + sizeof (ACPI_TABLE_HEADER))));

            ACPI_DUMP_BUFFER ((WalkState->ParserState.Aml - 16), 48);

#ifdef ACPI_ASL_COMPILER
             
            AcpiOsPrintf (
                "/*\nError: Unknown opcode 0x%.2X at table offset 0x%.4X, context:\n",
                WalkState->Opcode,
                (UINT32) (AmlOffset + sizeof (ACPI_TABLE_HEADER)));

            ACPI_ERROR ((AE_INFO,
                "Aborting disassembly, AML byte code is corrupt"));

             

            AcpiUtDumpBuffer (((UINT8 *) WalkState->ParserState.Aml - 16),
                48, DB_BYTE_DISPLAY,
                (AmlOffset + sizeof (ACPI_TABLE_HEADER) - 16));
            AcpiOsPrintf (" */\n");

             
            return_ACPI_STATUS (AE_AML_BAD_OPCODE);
#endif
        }

         

        WalkState->ParserState.Aml++;
        if (WalkState->Opcode > 0xFF)  
        {
            WalkState->ParserState.Aml++;
        }

        return_ACPI_STATUS (AE_CTRL_PARSE_CONTINUE);

    default:

         

        WalkState->ParserState.Aml +=
            AcpiPsGetOpcodeSize (WalkState->Opcode);
        WalkState->ArgTypes = WalkState->OpInfo->ParseArgs;
        break;
    }

    return_ACPI_STATUS (AE_OK);
}
