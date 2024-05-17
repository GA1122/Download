compile_quantifier_node(QuantNode* qn, regex_t* reg, ScanEnv* env)
{
  int i, r, mod_tlen;
  int infinite = IS_REPEAT_INFINITE(qn->upper);
  enum BodyEmpty empty_info = qn->empty_info;
  int tlen = compile_length_tree(NODE_QUANT_BODY(qn), reg);

  if (tlen < 0) return tlen;
  if (tlen == 0) return 0;

  if (is_anychar_infinite_greedy(qn) &&
      (qn->lower <= 1 ||
       int_multiply_cmp(tlen, qn->lower, QUANTIFIER_EXPAND_LIMIT_SIZE) <= 0)) {
    r = compile_tree_n_times(NODE_QUANT_BODY(qn), qn->lower, reg, env);
    if (r != 0) return r;
    if (IS_NOT_NULL(qn->next_head_exact)) {
      r = add_op(reg,
                 IS_MULTILINE(CTYPE_OPTION(NODE_QUANT_BODY(qn), reg)) ?
                 OP_ANYCHAR_ML_STAR_PEEK_NEXT : OP_ANYCHAR_STAR_PEEK_NEXT);
      if (r != 0) return r;

      COP(reg)->anychar_star_peek_next.c = STR_(qn->next_head_exact)->s[0];
      return 0;
    }
    else {
      r = add_op(reg,
                 IS_MULTILINE(CTYPE_OPTION(NODE_QUANT_BODY(qn), reg)) ?
                 OP_ANYCHAR_ML_STAR : OP_ANYCHAR_STAR);
      return r;
    }
  }

  if (empty_info == BODY_IS_NOT_EMPTY)
    mod_tlen = tlen;
  else
    mod_tlen = tlen + (SIZE_OP_EMPTY_CHECK_START + SIZE_OP_EMPTY_CHECK_END);

  if (infinite &&
      (qn->lower <= 1 ||
       int_multiply_cmp(tlen, qn->lower, QUANTIFIER_EXPAND_LIMIT_SIZE) <= 0)) {
    int addr;

    if (qn->lower == 1 && tlen > QUANTIFIER_EXPAND_LIMIT_SIZE) {
      r = add_op(reg, OP_JUMP);
      if (r != 0) return r;
      if (qn->greedy) {
#ifdef USE_OP_PUSH_OR_JUMP_EXACT
        if (IS_NOT_NULL(qn->head_exact))
          COP(reg)->jump.addr = SIZE_OP_PUSH_OR_JUMP_EXACT1 + SIZE_INC_OP;
        else
#endif
        if (IS_NOT_NULL(qn->next_head_exact))
          COP(reg)->jump.addr = SIZE_OP_PUSH_IF_PEEK_NEXT + SIZE_INC_OP;
        else
          COP(reg)->jump.addr = SIZE_OP_PUSH + SIZE_INC_OP;
      }
      else {
        COP(reg)->jump.addr = SIZE_OP_JUMP + SIZE_INC_OP;
      }
    }
    else {
      r = compile_tree_n_times(NODE_QUANT_BODY(qn), qn->lower, reg, env);
      if (r != 0) return r;
    }

    if (qn->greedy) {
#ifdef USE_OP_PUSH_OR_JUMP_EXACT
      if (IS_NOT_NULL(qn->head_exact)) {
        r = add_op(reg, OP_PUSH_OR_JUMP_EXACT1);
        if (r != 0) return r;
        COP(reg)->push_or_jump_exact1.addr = SIZE_INC_OP + mod_tlen + SIZE_OP_JUMP;
        COP(reg)->push_or_jump_exact1.c    = STR_(qn->head_exact)->s[0];

        r = compile_tree_empty_check(NODE_QUANT_BODY(qn), reg, empty_info, env);
        if (r != 0) return r;

        addr = -(mod_tlen + (int )SIZE_OP_PUSH_OR_JUMP_EXACT1);
      }
      else
#endif
      if (IS_NOT_NULL(qn->next_head_exact)) {
        r = add_op(reg, OP_PUSH_IF_PEEK_NEXT);
        if (r != 0) return r;
        COP(reg)->push_if_peek_next.addr = SIZE_INC_OP + mod_tlen + SIZE_OP_JUMP;
        COP(reg)->push_if_peek_next.c    = STR_(qn->next_head_exact)->s[0];

        r = compile_tree_empty_check(NODE_QUANT_BODY(qn), reg, empty_info, env);
        if (r != 0) return r;

        addr = -(mod_tlen + (int )SIZE_OP_PUSH_IF_PEEK_NEXT);
      }
      else {
        r = add_op(reg, OP_PUSH);
        if (r != 0) return r;
        COP(reg)->push.addr = SIZE_INC_OP + mod_tlen + SIZE_OP_JUMP;

        r = compile_tree_empty_check(NODE_QUANT_BODY(qn), reg, empty_info, env);
        if (r != 0) return r;

        addr = -(mod_tlen + (int )SIZE_OP_PUSH);
      }

      r = add_op(reg, OP_JUMP);
      if (r != 0) return r;
      COP(reg)->jump.addr = addr;
    }
    else {
      r = add_op(reg, OP_JUMP);
      if (r != 0) return r;
      COP(reg)->jump.addr = mod_tlen + SIZE_INC_OP;

      r = compile_tree_empty_check(NODE_QUANT_BODY(qn), reg, empty_info, env);
      if (r != 0) return r;

      r = add_op(reg, OP_PUSH);
      if (r != 0) return r;
      COP(reg)->push.addr = -mod_tlen;
    }
  }
  else if (qn->upper == 0) {
    if (qn->is_refered != 0) {  
      r = add_op(reg, OP_JUMP);
      if (r != 0) return r;
      COP(reg)->jump.addr = tlen + SIZE_INC_OP;

      r = compile_tree(NODE_QUANT_BODY(qn), reg, env);
    }
    else {
       
      r = 0;
    }
  }
  else if (! infinite && qn->greedy &&
           (qn->upper == 1 ||
            int_multiply_cmp(tlen + SIZE_OP_PUSH, qn->upper,
                             QUANTIFIER_EXPAND_LIMIT_SIZE) <= 0)) {
    int n = qn->upper - qn->lower;

    r = compile_tree_n_times(NODE_QUANT_BODY(qn), qn->lower, reg, env);
    if (r != 0) return r;

    for (i = 0; i < n; i++) {
      int v = onig_positive_int_multiply(n - i, tlen + SIZE_OP_PUSH);
      if (v < 0) return ONIGERR_TOO_BIG_NUMBER_FOR_REPEAT_RANGE;

      r = add_op(reg, OP_PUSH);
      if (r != 0) return r;
      COP(reg)->push.addr = v;

      r = compile_tree(NODE_QUANT_BODY(qn), reg, env);
      if (r != 0) return r;
    }
  }
  else if (! qn->greedy && qn->upper == 1 && qn->lower == 0) {  
    r = add_op(reg, OP_PUSH);
    if (r != 0) return r;
    COP(reg)->push.addr = SIZE_INC_OP + SIZE_OP_JUMP;

    r = add_op(reg, OP_JUMP);
    if (r != 0) return r;
    COP(reg)->jump.addr = tlen + SIZE_INC_OP;

    r = compile_tree(NODE_QUANT_BODY(qn), reg, env);
  }
  else {
    r = compile_range_repeat_node(qn, mod_tlen, empty_info, reg, env);
  }
  return r;
}