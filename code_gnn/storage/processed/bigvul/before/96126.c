static void timer_enter_waiting(Timer *t, bool initial) {
        bool found_monotonic = false, found_realtime = false;
        usec_t ts_realtime, ts_monotonic;
        usec_t base = 0;
        TimerValue *v;
        int r;

         

        ts_realtime = now(CLOCK_REALTIME);
        ts_monotonic = now(t->wake_system ? CLOCK_BOOTTIME : CLOCK_MONOTONIC);
        t->next_elapse_monotonic_or_boottime = t->next_elapse_realtime = 0;

        LIST_FOREACH(value, v, t->values) {

                if (v->disabled)
                        continue;

                if (v->base == TIMER_CALENDAR) {
                        usec_t b;

                         

                        b = t->last_trigger.realtime > 0 ? t->last_trigger.realtime : ts_realtime;

                        r = calendar_spec_next_usec(v->calendar_spec, b, &v->next_elapse);
                        if (r < 0)
                                continue;

                        if (!found_realtime)
                                t->next_elapse_realtime = v->next_elapse;
                        else
                                t->next_elapse_realtime = MIN(t->next_elapse_realtime, v->next_elapse);

                        found_realtime = true;

                } else  {
                        switch (v->base) {

                        case TIMER_ACTIVE:
                                if (state_translation_table[t->state] == UNIT_ACTIVE)
                                        base = UNIT(t)->inactive_exit_timestamp.monotonic;
                                else
                                        base = ts_monotonic;
                                break;

                        case TIMER_BOOT:
                                if (detect_container() <= 0) {
                                         
                                        base = 0;
                                        break;
                                }
                                 
                        case TIMER_STARTUP:
                                base = UNIT(t)->manager->userspace_timestamp.monotonic;
                                break;

                        case TIMER_UNIT_ACTIVE:

                                base = UNIT_TRIGGER(UNIT(t))->inactive_exit_timestamp.monotonic;

                                if (base <= 0)
                                        base = t->last_trigger.monotonic;

                                if (base <= 0)
                                        continue;

                                break;

                        case TIMER_UNIT_INACTIVE:

                                base = UNIT_TRIGGER(UNIT(t))->inactive_enter_timestamp.monotonic;

                                if (base <= 0)
                                        base = t->last_trigger.monotonic;

                                if (base <= 0)
                                        continue;

                                break;

                        default:
                                assert_not_reached("Unknown timer base");
                        }

                        if (t->wake_system)
                                base = monotonic_to_boottime(base);

                        v->next_elapse = base + v->value;

                        if (!initial && v->next_elapse < ts_monotonic && IN_SET(v->base, TIMER_ACTIVE, TIMER_BOOT, TIMER_STARTUP)) {
                                 
                                v->disabled = true;
                                continue;
                        }

                        if (!found_monotonic)
                                t->next_elapse_monotonic_or_boottime = v->next_elapse;
                        else
                                t->next_elapse_monotonic_or_boottime = MIN(t->next_elapse_monotonic_or_boottime, v->next_elapse);

                        found_monotonic = true;
                }
        }

        if (!found_monotonic && !found_realtime) {
                log_unit_debug(UNIT(t), "Timer is elapsed.");
                timer_set_state(t, TIMER_ELAPSED);
                return;
        }

        if (found_monotonic) {
                char buf[FORMAT_TIMESPAN_MAX];

                log_unit_debug(UNIT(t), "Monotonic timer elapses in %s.", format_timespan(buf, sizeof(buf), t->next_elapse_monotonic_or_boottime > ts_monotonic ? t->next_elapse_monotonic_or_boottime - ts_monotonic : 0, 0));

                if (t->monotonic_event_source) {
                        r = sd_event_source_set_time(t->monotonic_event_source, t->next_elapse_monotonic_or_boottime);
                        if (r < 0)
                                goto fail;

                        r = sd_event_source_set_enabled(t->monotonic_event_source, SD_EVENT_ONESHOT);
                        if (r < 0)
                                goto fail;
                } else {

                        r = sd_event_add_time(
                                        UNIT(t)->manager->event,
                                        &t->monotonic_event_source,
                                        t->wake_system ? CLOCK_BOOTTIME_ALARM : CLOCK_MONOTONIC,
                                        t->next_elapse_monotonic_or_boottime, t->accuracy_usec,
                                        timer_dispatch, t);
                        if (r < 0)
                                goto fail;

                        (void) sd_event_source_set_description(t->monotonic_event_source, "timer-monotonic");
                }

        } else if (t->monotonic_event_source) {

                r = sd_event_source_set_enabled(t->monotonic_event_source, SD_EVENT_OFF);
                if (r < 0)
                        goto fail;
        }

        if (found_realtime) {
                char buf[FORMAT_TIMESTAMP_MAX];
                log_unit_debug(UNIT(t), "Realtime timer elapses at %s.", format_timestamp(buf, sizeof(buf), t->next_elapse_realtime));

                if (t->realtime_event_source) {
                        r = sd_event_source_set_time(t->realtime_event_source, t->next_elapse_realtime);
                        if (r < 0)
                                goto fail;

                        r = sd_event_source_set_enabled(t->realtime_event_source, SD_EVENT_ONESHOT);
                        if (r < 0)
                                goto fail;
                } else {
                        r = sd_event_add_time(
                                        UNIT(t)->manager->event,
                                        &t->realtime_event_source,
                                        t->wake_system ? CLOCK_REALTIME_ALARM : CLOCK_REALTIME,
                                        t->next_elapse_realtime, t->accuracy_usec,
                                        timer_dispatch, t);
                        if (r < 0)
                                goto fail;

                        (void) sd_event_source_set_description(t->realtime_event_source, "timer-realtime");
                }

        } else if (t->realtime_event_source) {

                r = sd_event_source_set_enabled(t->realtime_event_source, SD_EVENT_OFF);
                if (r < 0)
                        goto fail;
        }

        timer_set_state(t, TIMER_WAITING);
        return;

fail:
        log_unit_warning_errno(UNIT(t), r, "Failed to enter waiting state: %m");
        timer_enter_dead(t, TIMER_FAILURE_RESOURCES);
}