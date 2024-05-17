        SelectionPin(IRCView *doc) : pos(0), anc(0), d(doc)
        {
            if (d->textCursor().hasSelection())
            {
                int end = d->document()->rootFrame()->lastPosition();

                d->document()->lastBlock();

                pos = d->textCursor().position();
                anc = d->textCursor().anchor();
                if (pos != end && anc != end)
                    anc = pos = 0;
            }
        }
