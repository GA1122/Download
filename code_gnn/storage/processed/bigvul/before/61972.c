AcpiPsBuildNamedOp (
    ACPI_WALK_STATE         *WalkState,
    UINT8                   *AmlOpStart,
    ACPI_PARSE_OBJECT       *UnnamedOp,
    ACPI_PARSE_OBJECT       **Op)
{
    ACPI_STATUS             Status = AE_OK;
    ACPI_PARSE_OBJECT       *Arg = NULL;


    ACPI_FUNCTION_TRACE_PTR (PsBuildNamedOp, WalkState);


    UnnamedOp->Common.Value.Arg = NULL;
    UnnamedOp->Common.ArgListLength = 0;
    UnnamedOp->Common.AmlOpcode = WalkState->Opcode;

     
    while (GET_CURRENT_ARG_TYPE (WalkState->ArgTypes) &&
          (GET_CURRENT_ARG_TYPE (WalkState->ArgTypes) != ARGP_NAME))
    {
        ASL_CV_CAPTURE_COMMENTS (WalkState);
        Status = AcpiPsGetNextArg (WalkState, &(WalkState->ParserState),
            GET_CURRENT_ARG_TYPE (WalkState->ArgTypes), &Arg);
        if (ACPI_FAILURE (Status))
        {
            return_ACPI_STATUS (Status);
        }

        AcpiPsAppendArg (UnnamedOp, Arg);
        INCREMENT_ARG_LIST (WalkState->ArgTypes);
    }

     

    ASL_CV_CAPTURE_COMMENTS (WalkState);

#ifdef ACPI_ASL_COMPILER
    if (AcpiGbl_CurrentInlineComment != NULL)
    {
        UnnamedOp->Common.NameComment = AcpiGbl_CurrentInlineComment;
        AcpiGbl_CurrentInlineComment = NULL;
    }
#endif

     
    if (!GET_CURRENT_ARG_TYPE (WalkState->ArgTypes))
    {
        return_ACPI_STATUS (AE_AML_NO_OPERAND);
    }

     

    INCREMENT_ARG_LIST (WalkState->ArgTypes);

     
    WalkState->Op = NULL;

    Status = WalkState->DescendingCallback (WalkState, Op);
    if (ACPI_FAILURE (Status))
    {
        if (Status != AE_CTRL_TERMINATE)
        {
            ACPI_EXCEPTION ((AE_INFO, Status, "During name lookup/catalog"));
        }
        return_ACPI_STATUS (Status);
    }

    if (!*Op)
    {
        return_ACPI_STATUS (AE_CTRL_PARSE_CONTINUE);
    }

    Status = AcpiPsNextParseState (WalkState, *Op, Status);
    if (ACPI_FAILURE (Status))
    {
        if (Status == AE_CTRL_PENDING)
        {
            Status = AE_CTRL_PARSE_PENDING;
        }
        return_ACPI_STATUS (Status);
    }

    AcpiPsAppendArg (*Op, UnnamedOp->Common.Value.Arg);

#ifdef ACPI_ASL_COMPILER

     

    (*Op)->Common.InlineComment     = UnnamedOp->Common.InlineComment;
    (*Op)->Common.EndNodeComment    = UnnamedOp->Common.EndNodeComment;
    (*Op)->Common.CloseBraceComment = UnnamedOp->Common.CloseBraceComment;
    (*Op)->Common.NameComment       = UnnamedOp->Common.NameComment;
    (*Op)->Common.CommentList       = UnnamedOp->Common.CommentList;
    (*Op)->Common.EndBlkComment     = UnnamedOp->Common.EndBlkComment;
    (*Op)->Common.CvFilename        = UnnamedOp->Common.CvFilename;
    (*Op)->Common.CvParentFilename  = UnnamedOp->Common.CvParentFilename;
    (*Op)->Named.Aml                = UnnamedOp->Common.Aml;

    UnnamedOp->Common.InlineComment     = NULL;
    UnnamedOp->Common.EndNodeComment    = NULL;
    UnnamedOp->Common.CloseBraceComment = NULL;
    UnnamedOp->Common.NameComment       = NULL;
    UnnamedOp->Common.CommentList       = NULL;
    UnnamedOp->Common.EndBlkComment     = NULL;
#endif

    if ((*Op)->Common.AmlOpcode == AML_REGION_OP ||
        (*Op)->Common.AmlOpcode == AML_DATA_REGION_OP)
    {
         
        (*Op)->Named.Data = AmlOpStart;
        (*Op)->Named.Length = 0;
    }

    return_ACPI_STATUS (AE_OK);
}
