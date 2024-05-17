count_exec_stack(i_ctx_t *i_ctx_p, bool include_marks)
{
    uint count = ref_stack_count(&e_stack);

    if (!include_marks) {
        uint i;

        for (i = count; i--;)
            if (r_has_type_attrs(ref_stack_index(&e_stack, (long)i),
                                 t_null, a_executable))
                --count;
    }
    return count;
}
