
- focus(c): focus the c stack(windows)

- arrange(NULL): arrange(c) is immediately arrange the  windows layouts of the c monitor. arrange(NULL) is arrange all of monitor.

- restack(selmon): I don't know what is that. But if I don't add it in sendmon(), the cursor is not following the windows to move together.

- selmon->sel: if selmon->sel is NULL, the current tag/monitor don't have stack/windows.
