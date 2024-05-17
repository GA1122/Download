optimize_nodes(Node* node, OptNode* opt, OptEnv* env)
{
  int i;
  int r;
  OptNode xo;
  OnigEncoding enc;

  r = 0;
  enc = env->enc;
  clear_node_opt_info(opt);
  set_bound_node_opt_info(opt, &env->mmd);

  switch (NODE_TYPE(node)) {
  case NODE_LIST:
    {
      OptEnv nenv;
      Node* nd = node;

      copy_opt_env(&nenv, env);
      do {
        r = optimize_nodes(NODE_CAR(nd), &xo, &nenv);
        if (r == 0) {
          add_mml(&nenv.mmd, &xo.len);
          concat_left_node_opt_info(enc, opt, &xo);
        }
      } while (r == 0 && IS_NOT_NULL(nd = NODE_CDR(nd)));
    }
    break;

  case NODE_ALT:
    {
      Node* nd = node;

      do {
        r = optimize_nodes(NODE_CAR(nd), &xo, env);
        if (r == 0) {
          if (nd == node) copy_node_opt_info(opt, &xo);
          else            alt_merge_node_opt_info(opt, &xo, env);
        }
      } while ((r == 0) && IS_NOT_NULL(nd = NODE_CDR(nd)));
    }
    break;

  case NODE_STRING:
    {
      StrNode* sn = STR_(node);
      int slen = (int )(sn->end - sn->s);
       

      if (! NODE_STRING_IS_AMBIG(node)) {
        concat_opt_exact_str(&opt->sb, sn->s, sn->end, enc);
        if (slen > 0) {
          add_char_opt_map(&opt->map, *(sn->s), enc);
        }
        set_mml(&opt->len, slen, slen);
      }
      else {
        int max;

        if (NODE_STRING_IS_DONT_GET_OPT_INFO(node)) {
          int n = onigenc_strlen(enc, sn->s, sn->end);
          max = ONIGENC_MBC_MAXLEN_DIST(enc) * n;
        }
        else {
          concat_opt_exact_str(&opt->sb, sn->s, sn->end, enc);
          opt->sb.case_fold = 1;
          if (NODE_STRING_IS_GOOD_AMBIG(node))
            opt->sb.good_case_fold = 1;

          if (slen > 0) {
            r = add_char_amb_opt_map(&opt->map, sn->s, sn->end,
                                     enc, env->case_fold_flag);
            if (r != 0) break;
          }

          max = slen;
        }

        set_mml(&opt->len, slen, max);
      }
    }
    break;

  case NODE_CCLASS:
    {
      int z;
      CClassNode* cc = CCLASS_(node);

       

      if (IS_NOT_NULL(cc->mbuf) || IS_NCCLASS_NOT(cc)) {
        OnigLen min = ONIGENC_MBC_MINLEN(enc);
        OnigLen max = ONIGENC_MBC_MAXLEN_DIST(enc);

        set_mml(&opt->len, min, max);
      }
      else {
        for (i = 0; i < SINGLE_BYTE_SIZE; i++) {
          z = BITSET_AT(cc->bs, i);
          if ((z && ! IS_NCCLASS_NOT(cc)) || (! z && IS_NCCLASS_NOT(cc))) {
            add_char_opt_map(&opt->map, (UChar )i, enc);
          }
        }
        set_mml(&opt->len, 1, 1);
      }
    }
    break;

  case NODE_CTYPE:
    {
      int min, max;
      int range;

      max = ONIGENC_MBC_MAXLEN_DIST(enc);

      if (max == 1) {
        min = 1;

        switch (CTYPE_(node)->ctype) {
        case CTYPE_ANYCHAR:
          break;

        case ONIGENC_CTYPE_WORD:
          range = CTYPE_(node)->ascii_mode != 0 ? 128 : SINGLE_BYTE_SIZE;
          if (CTYPE_(node)->not != 0) {
            for (i = 0; i < range; i++) {
              if (! ONIGENC_IS_CODE_WORD(enc, i)) {
                add_char_opt_map(&opt->map, (UChar )i, enc);
              }
            }
            for (i = range; i < SINGLE_BYTE_SIZE; i++) {
              add_char_opt_map(&opt->map, (UChar )i, enc);
            }
          }
          else {
            for (i = 0; i < range; i++) {
              if (ONIGENC_IS_CODE_WORD(enc, i)) {
                add_char_opt_map(&opt->map, (UChar )i, enc);
              }
            }
          }
          break;
        }
      }
      else {
        min = ONIGENC_MBC_MINLEN(enc);
      }
      set_mml(&opt->len, min, max);
    }
    break;

  case NODE_ANCHOR:
    switch (ANCHOR_(node)->type) {
    case ANCR_BEGIN_BUF:
    case ANCR_BEGIN_POSITION:
    case ANCR_BEGIN_LINE:
    case ANCR_END_BUF:
    case ANCR_SEMI_END_BUF:
    case ANCR_END_LINE:
    case ANCR_PREC_READ_NOT:
    case ANCR_LOOK_BEHIND:
      add_opt_anc_info(&opt->anc, ANCHOR_(node)->type);
      break;

    case ANCR_PREC_READ:
      {
        r = optimize_nodes(NODE_BODY(node), &xo, env);
        if (r == 0) {
          if (xo.sb.len > 0)
            copy_opt_exact(&opt->spr, &xo.sb);
          else if (xo.sm.len > 0)
            copy_opt_exact(&opt->spr, &xo.sm);

          opt->spr.reach_end = 0;

          if (xo.map.value > 0)
            copy_opt_map(&opt->map, &xo.map);
        }
      }
      break;

    case ANCR_LOOK_BEHIND_NOT:
      break;
    }
    break;

  case NODE_BACKREF:
    if (! NODE_IS_CHECKER(node)) {
      int* backs;
      OnigLen min, max, tmin, tmax;
      MemEnv* mem_env = SCANENV_MEMENV(env->scan_env);
      BackRefNode* br = BACKREF_(node);

      if (NODE_IS_RECURSION(node)) {
        set_mml(&opt->len, 0, INFINITE_LEN);
        break;
      }
      backs = BACKREFS_P(br);
      min = tree_min_len(mem_env[backs[0]].node, env->scan_env);
      max = tree_max_len(mem_env[backs[0]].node, env->scan_env);
      for (i = 1; i < br->back_num; i++) {
        tmin = tree_min_len(mem_env[backs[i]].node, env->scan_env);
        tmax = tree_max_len(mem_env[backs[i]].node, env->scan_env);
        if (min > tmin) min = tmin;
        if (max < tmax) max = tmax;
      }
      set_mml(&opt->len, min, max);
    }
    break;

#ifdef USE_CALL
  case NODE_CALL:
    if (NODE_IS_RECURSION(node))
      set_mml(&opt->len, 0, INFINITE_LEN);
    else {
      OnigOptionType save = env->options;
      env->options = BAG_(NODE_BODY(node))->o.options;
      r = optimize_nodes(NODE_BODY(node), opt, env);
      env->options = save;
    }
    break;
#endif

  case NODE_QUANT:
    {
      OnigLen min, max;
      QuantNode* qn = QUANT_(node);

      r = optimize_nodes(NODE_BODY(node), &xo, env);
      if (r != 0) break;

      if (qn->lower > 0) {
        copy_node_opt_info(opt, &xo);
        if (xo.sb.len > 0) {
          if (xo.sb.reach_end) {
            for (i = 2; i <= qn->lower && ! is_full_opt_exact(&opt->sb); i++) {
              int rc = concat_opt_exact(&opt->sb, &xo.sb, enc);
              if (rc > 0) break;
            }
            if (i < qn->lower) opt->sb.reach_end = 0;
          }
        }

        if (qn->lower != qn->upper) {
          opt->sb.reach_end = 0;
          opt->sm.reach_end = 0;
        }
        if (qn->lower > 1)
          opt->sm.reach_end = 0;
      }

      if (IS_REPEAT_INFINITE(qn->upper)) {
        if (env->mmd.max == 0 &&
            NODE_IS_ANYCHAR(NODE_BODY(node)) && qn->greedy != 0) {
          if (IS_MULTILINE(CTYPE_OPTION(NODE_QUANT_BODY(qn), env)))
            add_opt_anc_info(&opt->anc, ANCR_ANYCHAR_INF_ML);
          else
            add_opt_anc_info(&opt->anc, ANCR_ANYCHAR_INF);
        }

        max = (xo.len.max > 0 ? INFINITE_LEN : 0);
      }
      else {
        max = distance_multiply(xo.len.max, qn->upper);
      }

      min = distance_multiply(xo.len.min, qn->lower);
      set_mml(&opt->len, min, max);
    }
    break;

  case NODE_BAG:
    {
      BagNode* en = BAG_(node);

      switch (en->type) {
      case BAG_OPTION:
        {
          OnigOptionType save = env->options;

          env->options = en->o.options;
          r = optimize_nodes(NODE_BODY(node), opt, env);
          env->options = save;
        }
        break;

      case BAG_MEMORY:
#ifdef USE_CALL
        en->opt_count++;
        if (en->opt_count > MAX_NODE_OPT_INFO_REF_COUNT) {
          OnigLen min, max;

          min = 0;
          max = INFINITE_LEN;
          if (NODE_IS_MIN_FIXED(node)) min = en->min_len;
          if (NODE_IS_MAX_FIXED(node)) max = en->max_len;
          set_mml(&opt->len, min, max);
        }
        else
#endif
          {
            r = optimize_nodes(NODE_BODY(node), opt, env);
            if (is_set_opt_anc_info(&opt->anc, ANCR_ANYCHAR_INF_MASK)) {
              if (MEM_STATUS_AT0(env->scan_env->backrefed_mem, en->m.regnum))
                remove_opt_anc_info(&opt->anc, ANCR_ANYCHAR_INF_MASK);
            }
          }
        break;

      case BAG_STOP_BACKTRACK:
        r = optimize_nodes(NODE_BODY(node), opt, env);
        break;

      case BAG_IF_ELSE:
        {
          OptEnv nenv;

          copy_opt_env(&nenv, env);
          r = optimize_nodes(NODE_BAG_BODY(en), &xo, &nenv);
          if (r == 0) {
            add_mml(&nenv.mmd, &xo.len);
            concat_left_node_opt_info(enc, opt, &xo);
            if (IS_NOT_NULL(en->te.Then)) {
              r = optimize_nodes(en->te.Then, &xo, &nenv);
              if (r == 0) {
                concat_left_node_opt_info(enc, opt, &xo);
              }
            }

            if (IS_NOT_NULL(en->te.Else)) {
              r = optimize_nodes(en->te.Else, &xo, env);
              if (r == 0)
                alt_merge_node_opt_info(opt, &xo, env);
            }
          }
        }
        break;
      }
    }
    break;

  case NODE_GIMMICK:
    break;

  default:
#ifdef ONIG_DEBUG
    fprintf(stderr, "optimize_nodes: undefined node type %d\n", NODE_TYPE(node));
#endif
    r = ONIGERR_TYPE_BUG;
    break;
  }

  return r;
}