#include <gtk/gtk.h>
#include <glib/gprintf.h>

static void
paste_ready (
  GdkClipboard *const clipboard,
  GAsyncResult *const result,
  gpointer      const user_data
) {
  g_autofree gchar *text = gdk_clipboard_read_text_finish (
    clipboard,
    result,
    NULL
  );

  gchar **const last_paste = user_data;

  if (text == NULL) {
    g_debug ("no text in clipboard");
    return;
  }

  if (g_strcmp0 (*last_paste, text) == 0) {
    g_debug ("repeat of `%s', skipping", text);
    return;
  }

  g_free (*last_paste);
  *last_paste = g_strdup (text);

  g_fprintf (stdout, "%s\n", text);
  fflush (stdout);
}

static void
clipboard_changed (
  GdkClipboard *const clipboard,
  gpointer      const user_data
) {
  g_debug ("clipboard changed, pasting");

  gdk_clipboard_read_text_async (
    clipboard,
    NULL,
    (GAsyncReadyCallback) paste_ready,
    user_data
  );
}

static void
on_activate (
  GApplication *const app,
  gpointer      const user_data
) {
  GtkWidget *const window = gtk_application_window_new (GTK_APPLICATION (app));
  GdkClipboard *const clipboard = gtk_widget_get_clipboard (window);

  g_signal_connect (
    clipboard,
    "changed",
    G_CALLBACK (clipboard_changed),
    user_data
  );
}

gint
main (
  gint argc,
  gchar **argv
) {
  g_autoptr(GtkApplication) app = gtk_application_new (
    "me.yelloz.jordan.ClipboardWatcher",
    0
  );

  g_autofree gchar *last_paste = NULL;

  /* XXX: Clipboard can't be passively monitored with Wayland at the moment.
   * Without this, the clipboard event will only fire when the watcher comes
   * into focus */
  g_setenv ("GDK_BACKEND", "x11", TRUE);

  g_signal_connect (
    app,
    "activate",
    G_CALLBACK (on_activate),
    &last_paste
  );

  return g_application_run (
    G_APPLICATION (app),
    argc,
    argv
  );
}
