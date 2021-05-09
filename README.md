# gtk-clipboard-watcher

Watches your X11 clipboard for changes, prints each to stdout.

This can be useful when manually browsing the web, finding links, and wishing to
asynchronously post-process them. In this case, all you need to do is start this
program and copy every link you want to process to the clipboard as you
encounter them. This tool combined with any UNIX pipeline you attach to the end
of it will do the rest.

Currently it relies on the X11 backend of GDK since at the time it was written,
the Wayland backend only responds to changes once a window from this program
goes into focus.
