gs_interp_free_stacks(gs_ref_memory_t * smem, gs_context_state_t * pcst)
{
     
    ref_stack_release(&pcst->dict_stack.stack);
    ref_stack_release(&pcst->exec_stack.stack);
    ref_stack_release(&pcst->op_stack.stack);
}
