pdf_process_keyword(fz_context *ctx, pdf_processor *proc, pdf_csi *csi, fz_stream *stm, char *word)
{
	float *s = csi->stack;
	int key;

	key = word[0];
	if (word[1])
	{
		key |= word[1] << 8;
		if (word[2])
		{
			key |= word[2] << 16;
			if (word[3])
				key = 0;
		}
	}

	switch (key)
	{
	default:
		if (!csi->xbalance)
			fz_throw(ctx, FZ_ERROR_SYNTAX, "unknown keyword: '%s'", word);
		break;

	 
	case A('w'): if (proc->op_w) proc->op_w(ctx, proc, s[0]); break;
	case A('j'): if (proc->op_j) proc->op_j(ctx, proc, fz_clampi(s[0], 0, 2)); break;
	case A('J'): if (proc->op_J) proc->op_J(ctx, proc, fz_clampi(s[0], 0, 2)); break;
	case A('M'): if (proc->op_M) proc->op_M(ctx, proc, s[0]); break;
	case A('d'): if (proc->op_d) proc->op_d(ctx, proc, csi->obj, s[0]); break;
	case B('r','i'): if (proc->op_ri) proc->op_ri(ctx, proc, csi->name); break;
	case A('i'): if (proc->op_i) proc->op_i(ctx, proc, s[0]); break;

	case B('g','s'):
		{
			pdf_obj *gsres, *gsobj;
			gsres = pdf_dict_get(ctx, csi->rdb, PDF_NAME_ExtGState);
			if (!gsres)
				fz_throw(ctx, FZ_ERROR_SYNTAX, "cannot find ExtGState dictionary");
			gsobj = pdf_dict_gets(ctx, gsres, csi->name);
			if (!gsobj)
				fz_throw(ctx, FZ_ERROR_SYNTAX, "cannot find ExtGState resource '%s'", csi->name);
			if (proc->op_gs_begin)
				proc->op_gs_begin(ctx, proc, csi->name, gsobj);
			pdf_process_extgstate(ctx, proc, csi, gsobj);
			if (proc->op_gs_end)
				proc->op_gs_end(ctx, proc);
		}
		break;

	 
	case A('q'): pdf_process_gsave(ctx, proc, csi); break;
	case A('Q'): pdf_process_grestore(ctx, proc, csi); break;
	case B('c','m'): if (proc->op_cm) proc->op_cm(ctx, proc, s[0], s[1], s[2], s[3], s[4], s[5]); break;

	 
	case A('m'): if (proc->op_m) proc->op_m(ctx, proc, s[0], s[1]); break;
	case A('l'): if (proc->op_l) proc->op_l(ctx, proc, s[0], s[1]); break;
	case A('c'): if (proc->op_c) proc->op_c(ctx, proc, s[0], s[1], s[2], s[3], s[4], s[5]); break;
	case A('v'): if (proc->op_v) proc->op_v(ctx, proc, s[0], s[1], s[2], s[3]); break;
	case A('y'): if (proc->op_y) proc->op_y(ctx, proc, s[0], s[1], s[2], s[3]); break;
	case A('h'): if (proc->op_h) proc->op_h(ctx, proc); break;
	case B('r','e'): if (proc->op_re) proc->op_re(ctx, proc, s[0], s[1], s[2], s[3]); break;

	 
	case A('S'): if (proc->op_S) proc->op_S(ctx, proc); break;
	case A('s'): if (proc->op_s) proc->op_s(ctx, proc); break;
	case A('F'): if (proc->op_F) proc->op_F(ctx, proc); break;
	case A('f'): if (proc->op_f) proc->op_f(ctx, proc); break;
	case B('f','*'): if (proc->op_fstar) proc->op_fstar(ctx, proc); break;
	case A('B'): if (proc->op_B) proc->op_B(ctx, proc); break;
	case B('B','*'): if (proc->op_Bstar) proc->op_Bstar(ctx, proc); break;
	case A('b'): if (proc->op_b) proc->op_b(ctx, proc); break;
	case B('b','*'): if (proc->op_bstar) proc->op_bstar(ctx, proc); break;
	case A('n'): if (proc->op_n) proc->op_n(ctx, proc); break;

	 
	case A('W'): if (proc->op_W) proc->op_W(ctx, proc); break;
	case B('W','*'): if (proc->op_Wstar) proc->op_Wstar(ctx, proc); break;

	 
	case B('B','T'): csi->in_text = 1; if (proc->op_BT) proc->op_BT(ctx, proc); break;
	case B('E','T'): csi->in_text = 0; if (proc->op_ET) proc->op_ET(ctx, proc); break;

	 
	case B('T','c'): if (proc->op_Tc) proc->op_Tc(ctx, proc, s[0]); break;
	case B('T','w'): if (proc->op_Tw) proc->op_Tw(ctx, proc, s[0]); break;
	case B('T','z'): if (proc->op_Tz) proc->op_Tz(ctx, proc, s[0]); break;
	case B('T','L'): if (proc->op_TL) proc->op_TL(ctx, proc, s[0]); break;
	case B('T','r'): if (proc->op_Tr) proc->op_Tr(ctx, proc, s[0]); break;
	case B('T','s'): if (proc->op_Ts) proc->op_Ts(ctx, proc, s[0]); break;

	case B('T','f'):
		if (proc->op_Tf)
		{
			pdf_obj *fontres, *fontobj;
			pdf_font_desc *font;
			fontres = pdf_dict_get(ctx, csi->rdb, PDF_NAME_Font);
			if (!fontres)
				fz_throw(ctx, FZ_ERROR_SYNTAX, "cannot find Font dictionary");
			fontobj = pdf_dict_gets(ctx, fontres, csi->name);
			if (!fontobj)
				fz_throw(ctx, FZ_ERROR_SYNTAX, "cannot find Font resource '%s'", csi->name);
			font = load_font_or_hail_mary(ctx, csi->doc, csi->rdb, fontobj, 0, csi->cookie);
			fz_try(ctx)
				proc->op_Tf(ctx, proc, csi->name, font, s[0]);
			fz_always(ctx)
				pdf_drop_font(ctx, font);
			fz_catch(ctx)
				fz_rethrow(ctx);
		}
		break;

	 
	case B('T','d'): if (proc->op_Td) proc->op_Td(ctx, proc, s[0], s[1]); break;
	case B('T','D'): if (proc->op_TD) proc->op_TD(ctx, proc, s[0], s[1]); break;
	case B('T','m'): if (proc->op_Tm) proc->op_Tm(ctx, proc, s[0], s[1], s[2], s[3], s[4], s[5]); break;
	case B('T','*'): if (proc->op_Tstar) proc->op_Tstar(ctx, proc); break;

	 
	case B('T','J'): if (proc->op_TJ) proc->op_TJ(ctx, proc, csi->obj); break;
	case B('T','j'):
		if (proc->op_Tj)
		{
			if (csi->string_len > 0)
				proc->op_Tj(ctx, proc, csi->string, csi->string_len);
			else
				proc->op_Tj(ctx, proc, pdf_to_str_buf(ctx, csi->obj), pdf_to_str_len(ctx, csi->obj));
		}
		break;
	case A('\''):
		if (proc->op_squote)
		{
			if (csi->string_len > 0)
				proc->op_squote(ctx, proc, csi->string, csi->string_len);
			else
				proc->op_squote(ctx, proc, pdf_to_str_buf(ctx, csi->obj), pdf_to_str_len(ctx, csi->obj));
		}
		break;
	case A('"'):
		if (proc->op_dquote)
		{
			if (csi->string_len > 0)
				proc->op_dquote(ctx, proc, s[0], s[1], csi->string, csi->string_len);
			else
				proc->op_dquote(ctx, proc, s[0], s[1], pdf_to_str_buf(ctx, csi->obj), pdf_to_str_len(ctx, csi->obj));
		}
		break;

	 
	case B('d','0'): if (proc->op_d0) proc->op_d0(ctx, proc, s[0], s[1]); break;
	case B('d','1'): if (proc->op_d1) proc->op_d1(ctx, proc, s[0], s[1], s[2], s[3], s[4], s[5]); break;

	 
	case B('C','S'): pdf_process_CS(ctx, proc, csi, 1); break;
	case B('c','s'): pdf_process_CS(ctx, proc, csi, 0); break;
	case B('S','C'): pdf_process_SC(ctx, proc, csi, 1); break;
	case B('s','c'): pdf_process_SC(ctx, proc, csi, 0); break;
	case C('S','C','N'): pdf_process_SC(ctx, proc, csi, 1); break;
	case C('s','c','n'): pdf_process_SC(ctx, proc, csi, 0); break;

	case A('G'): if (proc->op_G) proc->op_G(ctx, proc, s[0]); break;
	case A('g'): if (proc->op_g) proc->op_g(ctx, proc, s[0]); break;
	case B('R','G'): if (proc->op_RG) proc->op_RG(ctx, proc, s[0], s[1], s[2]); break;
	case B('r','g'): if (proc->op_rg) proc->op_rg(ctx, proc, s[0], s[1], s[2]); break;
	case A('K'): if (proc->op_K) proc->op_K(ctx, proc, s[0], s[1], s[2], s[3]); break;
	case A('k'): if (proc->op_k) proc->op_k(ctx, proc, s[0], s[1], s[2], s[3]); break;

	 
	case B('B','I'):
		{
			fz_image *img = parse_inline_image(ctx, csi, stm);
			fz_try(ctx)
			{
				if (proc->op_BI)
					proc->op_BI(ctx, proc, img);
			}
			fz_always(ctx)
				fz_drop_image(ctx, img);
			fz_catch(ctx)
				fz_rethrow(ctx);
		}
		break;

	case B('s','h'):
		if (proc->op_sh)
		{
			pdf_obj *shaderes, *shadeobj;
			fz_shade *shade;
			shaderes = pdf_dict_get(ctx, csi->rdb, PDF_NAME_Shading);
			if (!shaderes)
				fz_throw(ctx, FZ_ERROR_SYNTAX, "cannot find Shading dictionary");
			shadeobj = pdf_dict_gets(ctx, shaderes, csi->name);
			if (!shadeobj)
				fz_throw(ctx, FZ_ERROR_SYNTAX, "cannot find Shading resource '%s'", csi->name);
			shade = pdf_load_shading(ctx, csi->doc, shadeobj);
			fz_try(ctx)
				proc->op_sh(ctx, proc, csi->name, shade);
			fz_always(ctx)
				fz_drop_shade(ctx, shade);
			fz_catch(ctx)
				fz_rethrow(ctx);
		}
		break;

	case B('D','o'): pdf_process_Do(ctx, proc, csi); break;

	 
	case B('M','P'): if (proc->op_MP) proc->op_MP(ctx, proc, csi->name); break;
	case B('D','P'): if (proc->op_DP) proc->op_DP(ctx, proc, csi->name, csi->obj, resolve_properties(ctx, csi, csi->obj)); break;
	case C('B','M','C'): pdf_process_BMC(ctx, proc, csi, csi->name); break;
	case C('B','D','C'): pdf_process_BDC(ctx, proc, csi); break;
	case C('E','M','C'): pdf_process_EMC(ctx, proc, csi); break;

	 
	case B('B','X'): ++csi->xbalance; if (proc->op_BX) proc->op_BX(ctx, proc); break;
	case B('E','X'): --csi->xbalance; if (proc->op_EX) proc->op_EX(ctx, proc); break;
	}
}
