  AutofillInteractiveTest() :
      key_press_event_sink_(
          base::Bind(&AutofillInteractiveTest::HandleKeyPressEvent,
                     base::Unretained(this))) {}
