        ~SelectionPin()
        {
            if (d && (pos || anc))
            {
                QTextCursor mv(d->textCursor());
                mv.setPosition(anc);
                mv.setPosition(pos, QTextCursor::KeepAnchor);
                d->setTextCursor(mv);
            }
        }
