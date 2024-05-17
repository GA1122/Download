static void cstm(JF, js_Ast *stm)
{
	js_Ast *target;
	int loop, cont, then, end;

	emitline(J, F, stm);

	switch (stm->type) {
	case AST_FUNDEC:
		break;

	case STM_BLOCK:
		cstmlist(J, F, stm->a);
		break;

	case STM_EMPTY:
		if (F->script) {
			emitline(J, F, stm);
			emit(J, F, OP_POP);
			emit(J, F, OP_UNDEF);
		}
		break;

	case STM_VAR:
		cvarinit(J, F, stm->a);
		break;

	case STM_IF:
		if (stm->c) {
			cexp(J, F, stm->a);
			emitline(J, F, stm);
			then = emitjump(J, F, OP_JTRUE);
			cstm(J, F, stm->c);
			emitline(J, F, stm);
			end = emitjump(J, F, OP_JUMP);
			label(J, F, then);
			cstm(J, F, stm->b);
			label(J, F, end);
		} else {
			cexp(J, F, stm->a);
			emitline(J, F, stm);
			end = emitjump(J, F, OP_JFALSE);
			cstm(J, F, stm->b);
			label(J, F, end);
		}
		break;

	case STM_DO:
		loop = here(J, F);
		cstm(J, F, stm->a);
		cont = here(J, F);
		cexp(J, F, stm->b);
		emitline(J, F, stm);
		emitjumpto(J, F, OP_JTRUE, loop);
		labeljumps(J, F, stm->jumps, here(J,F), cont);
		break;

	case STM_WHILE:
		loop = here(J, F);
		cexp(J, F, stm->a);
		emitline(J, F, stm);
		end = emitjump(J, F, OP_JFALSE);
		cstm(J, F, stm->b);
		emitline(J, F, stm);
		emitjumpto(J, F, OP_JUMP, loop);
		label(J, F, end);
		labeljumps(J, F, stm->jumps, here(J,F), loop);
		break;

	case STM_FOR:
	case STM_FOR_VAR:
		if (stm->type == STM_FOR_VAR) {
			cvarinit(J, F, stm->a);
		} else {
			if (stm->a) {
				cexp(J, F, stm->a);
				emit(J, F, OP_POP);
			}
		}
		loop = here(J, F);
		if (stm->b) {
			cexp(J, F, stm->b);
			emitline(J, F, stm);
			end = emitjump(J, F, OP_JFALSE);
		} else {
			end = 0;
		}
		cstm(J, F, stm->d);
		cont = here(J, F);
		if (stm->c) {
			cexp(J, F, stm->c);
			emit(J, F, OP_POP);
		}
		emitline(J, F, stm);
		emitjumpto(J, F, OP_JUMP, loop);
		if (end)
			label(J, F, end);
		labeljumps(J, F, stm->jumps, here(J,F), cont);
		break;

	case STM_FOR_IN:
	case STM_FOR_IN_VAR:
		cexp(J, F, stm->b);
		emitline(J, F, stm);
		emit(J, F, OP_ITERATOR);
		loop = here(J, F);
		{
			emitline(J, F, stm);
			emit(J, F, OP_NEXTITER);
			end = emitjump(J, F, OP_JFALSE);
			cassignforin(J, F, stm);
			if (F->script) {
				emit(J, F, OP_ROT2);
				cstm(J, F, stm->c);
				emit(J, F, OP_ROT2);
			} else {
				cstm(J, F, stm->c);
			}
			emitline(J, F, stm);
			emitjumpto(J, F, OP_JUMP, loop);
		}
		label(J, F, end);
		labeljumps(J, F, stm->jumps, here(J,F), loop);
		break;

	case STM_SWITCH:
		cswitch(J, F, stm->a, stm->b);
		labeljumps(J, F, stm->jumps, here(J,F), 0);
		break;

	case STM_LABEL:
		cstm(J, F, stm->b);
		 
		while (stm->type == STM_LABEL)
			stm = stm->b;
		 
		if (!isloop(stm->type) && stm->type != STM_SWITCH)
			labeljumps(J, F, stm->jumps, here(J,F), 0);
		break;

	case STM_BREAK:
		if (stm->a) {
			checkfutureword(J, F, stm->a);
			target = breaktarget(J, F, stm->parent, stm->a->string);
			if (!target)
				jsC_error(J, stm, "break label '%s' not found", stm->a->string);
		} else {
			target = breaktarget(J, F, stm->parent, NULL);
			if (!target)
				jsC_error(J, stm, "unlabelled break must be inside loop or switch");
		}
		cexit(J, F, STM_BREAK, stm, target);
		emitline(J, F, stm);
		addjump(J, F, STM_BREAK, target, emitjump(J, F, OP_JUMP));
		break;

	case STM_CONTINUE:
		if (stm->a) {
			checkfutureword(J, F, stm->a);
			target = continuetarget(J, F, stm->parent, stm->a->string);
			if (!target)
				jsC_error(J, stm, "continue label '%s' not found", stm->a->string);
		} else {
			target = continuetarget(J, F, stm->parent, NULL);
			if (!target)
				jsC_error(J, stm, "continue must be inside loop");
		}
		cexit(J, F, STM_CONTINUE, stm, target);
		emitline(J, F, stm);
		addjump(J, F, STM_CONTINUE, target, emitjump(J, F, OP_JUMP));
		break;

	case STM_RETURN:
		if (stm->a)
			cexp(J, F, stm->a);
		else
			emit(J, F, OP_UNDEF);
		target = returntarget(J, F, stm->parent);
		if (!target)
			jsC_error(J, stm, "return not in function");
		cexit(J, F, STM_RETURN, stm, target);
		emitline(J, F, stm);
		emit(J, F, OP_RETURN);
		break;

	case STM_THROW:
		cexp(J, F, stm->a);
		emitline(J, F, stm);
		emit(J, F, OP_THROW);
		break;

	case STM_WITH:
		F->lightweight = 0;
		if (F->strict)
			jsC_error(J, stm->a, "'with' statements are not allowed in strict mode");
		cexp(J, F, stm->a);
		emitline(J, F, stm);
		emit(J, F, OP_WITH);
		cstm(J, F, stm->b);
		emitline(J, F, stm);
		emit(J, F, OP_ENDWITH);
		break;

	case STM_TRY:
		emitline(J, F, stm);
		if (stm->b && stm->c) {
			F->lightweight = 0;
			if (stm->d)
				ctrycatchfinally(J, F, stm->a, stm->b, stm->c, stm->d);
			else
				ctrycatch(J, F, stm->a, stm->b, stm->c);
		} else {
			ctryfinally(J, F, stm->a, stm->d);
		}
		break;

	case STM_DEBUGGER:
		emitline(J, F, stm);
		emit(J, F, OP_DEBUGGER);
		break;

	default:
		if (F->script) {
			emitline(J, F, stm);
			emit(J, F, OP_POP);
			cexp(J, F, stm);
		} else {
			cexp(J, F, stm);
			emitline(J, F, stm);
			emit(J, F, OP_POP);
		}
		break;
	}
}