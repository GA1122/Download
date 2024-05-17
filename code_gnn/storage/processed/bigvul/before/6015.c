e1000e_msix_notify_one(E1000ECore *core, uint32_t cause, uint32_t int_cfg)
{
    uint32_t effective_eiac;

    if (E1000_IVAR_ENTRY_VALID(int_cfg)) {
        uint32_t vec = E1000_IVAR_ENTRY_VEC(int_cfg);
        if (vec < E1000E_MSIX_VEC_NUM) {
            if (!e1000e_eitr_should_postpone(core, vec)) {
                trace_e1000e_irq_msix_notify_vec(vec);
                msix_notify(core->owner, vec);
            }
        } else {
            trace_e1000e_wrn_msix_vec_wrong(cause, int_cfg);
        }
    } else {
        trace_e1000e_wrn_msix_invalid(cause, int_cfg);
    }

    if (core->mac[CTRL_EXT] & E1000_CTRL_EXT_EIAME) {
        trace_e1000e_irq_iam_clear_eiame(core->mac[IAM], cause);
        core->mac[IAM] &= ~cause;
    }

    trace_e1000e_irq_icr_clear_eiac(core->mac[ICR], core->mac[EIAC]);

    effective_eiac = core->mac[EIAC] & cause;

    if (effective_eiac == E1000_ICR_OTHER) {
        effective_eiac |= E1000_ICR_OTHER_CAUSES;
    }

    core->mac[ICR] &= ~effective_eiac;

    if (!(core->mac[CTRL_EXT] & E1000_CTRL_EXT_IAME)) {
        core->mac[IMS] &= ~effective_eiac;
    }
}
