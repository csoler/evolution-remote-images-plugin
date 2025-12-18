/* latex-converter plugin for Gnome Evolution
 * Copyright (C) 2016 Red Hat, Inc. (www.redhat.com)
 * Copyright (C) 2025 Cyril Soler.
 *
 * Authors and maintainers:
 *           2016: Red Hat, Inc. (evolution plugin example code)
 *     April 2025: Cyril Soler <cyril.soler@inria.fr>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib/gi18n-lib.h>
#include <gtk/gtk.h>

#include <composer/e-msg-composer.h>

#include "remote-images-extension-gtk3.h"

#define LATEX_CONVERT_ACTION_NAME "convert-latex-equations-action"

// Plugin entry functions
//
gint e_plugin_lib_enable (EPlugin *, gint enable);

void e_plugin_ui_init (EPlugin *);

struct _MMsgComposerExtensionRemoteImagesPrivate {
	gint dummy;
};

G_DEFINE_DYNAMIC_TYPE_EXTENDED (MMsgComposerExtensionRemoteImages, m_msg_composer_extension_remote_images, E_TYPE_EXTENSION, 0, G_ADD_PRIVATE_DYNAMIC (MMsgComposerExtensionRemoteImages))

GtkWidget *e_plugin_lib_get_configure_widget (EPlugin *);

static void show_error_dialog(GtkWindow *parent, const char *message)
{
	GtkWidget *dialog;

	dialog = gtk_message_dialog_new(
			parent,
			GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_MESSAGE_ERROR,
			GTK_BUTTONS_CLOSE,
			"%s", message);

	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}
void show_error_dialog_scrollable(GtkWindow *parent, const char *title, const char *message)
{
	GtkWidget *dialog = gtk_dialog_new_with_buttons(
			title,
			parent,
			GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
			"_Close", GTK_RESPONSE_CLOSE,
			NULL);

	GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	// Create scrolled window
	GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_widget_set_size_request(scrolled_window, 400, 200); // set scroll area size

	// Create text view
	GtkWidget *text_view = gtk_text_view_new();
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);

	// Set the message text
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
	gtk_text_buffer_set_text(buffer, message, -1);

	// Add text view to scrolled window
	gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
	gtk_box_pack_start(GTK_BOX(content_area), scrolled_window, TRUE, TRUE, 10);

	gtk_widget_show_all(dialog);

	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

// Plugin implementation
//
gint e_plugin_lib_enable (EPlugin * /*ep*/, gint /*enable*/)
{
	return 0;
}

void e_plugin_ui_init (EPlugin * /*plugin*/)
{
}

static gboolean clipboard_has_image_url(void)
{
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gchar *text = gtk_clipboard_wait_for_text(clipboard);
    gboolean has_image = FALSE;

            printf("Examining text \"%s\"\n",text);

    if (text) {
        gchar *lower = g_ascii_strdown(text, -1);
        if (g_str_has_suffix(lower, ".png") ||
            g_str_has_suffix(lower, ".jpg") ||
            g_str_has_suffix(lower, ".jpeg") ||
            g_str_has_suffix(lower, ".gif") ||
            g_str_has_suffix(lower, ".bmp") ||
            g_str_has_suffix(lower, ".webp")) {
            has_image = TRUE;
            printf("Image detected!");
        }
        else
            printf("No image detected!");

        g_free(lower);
        g_free(text);
    }

    return has_image;
}
/* Called just before the menu is shown */
static void update_menu_item_sensitivity(GtkWidget *menu, gpointer user_data)
{
    GtkWidget *menu_item = GTK_WIDGET(user_data);
    gboolean enable = clipboard_has_image_url();

    printf("Setting sensitiviy to %d.\n",enable);

    gtk_widget_set_sensitive(menu_item, enable);
}
static gchar *create_remote_image_link(const gchar *url)
{
    int n = strlen(url);
    gchar *res = g_malloc(n*2+1000);

    snprintf(res,n+1000,"<img src=\"%s\" alt=\"Image link to %s\"/>",url,url);
    return res;
}

static void paste_special_cb (GtkAction *action, gpointer user_data)
{
    EMsgComposer *composer = user_data;
    EHTMLEditor *editor;
    GtkClipboard *clipboard;

    editor = e_msg_composer_get_editor (composer);
    if (!editor)
        return;

    clipboard = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);
    gchar *text = gtk_clipboard_wait_for_text (clipboard);

    if (!text)
        return;

    printf("Handling the following text: \"%s\"\n", text);

    gchar *html = create_remote_image_link(text);
    g_free (text);

    //
    EContentEditor *cnt_editor = e_html_editor_get_content_editor(editor);
    EContentEditorMode mode = e_html_editor_get_mode(editor);

    if(mode != E_CONTENT_EDITOR_MODE_HTML && mode != E_CONTENT_EDITOR_MODE_MARKDOWN_HTML)
    {
        show_error_dialog(&composer->parent,"Cannot convert equations in plain text mode.\nPlease use HTML editor instead.");
        g_free(html);
        return;
    }

    // Start async request of editor data

    printf("Inserting link: \"%s\".\n",html);

    e_content_editor_insert_content ( cnt_editor, html, E_CONTENT_EDITOR_INSERT_TEXT_HTML );

    g_print ("New 2 action: %s: for composer '%s'\n", G_STRFUNC, gtk_window_get_title (GTK_WINDOW (composer)));

    g_free(html);
}
static void m_msg_composer_extension_add_paste_action (MMsgComposerExtensionRemoteImages *msg_composer_ext, EMsgComposer *composer)
{
    printf("adding new action...");
    EHTMLEditor *editor = e_msg_composer_get_editor(composer);
    GtkUIManager *ui_manager = e_html_editor_get_ui_manager (editor);

    GtkActionGroup *group = gtk_action_group_new ("mypaste-actions");

    GtkActionEntry actions[] = {
        {
            "paste-special-action",
            NULL,
            "Paste URL as remote image",
            NULL,
            "Paste with custom HTML handling",
            G_CALLBACK (paste_special_cb)
        }
    };

    gtk_action_group_add_actions ( group, actions, G_N_ELEMENTS (actions), composer );

    gtk_ui_manager_insert_action_group (ui_manager, group, 0);
    g_object_unref (group);

    /* Now add the menu item directly to the popup menu */
    GtkWidget *popup = gtk_ui_manager_get_widget(ui_manager, "/ui/context-menu");
    if (popup) {
        GtkWidget *menu_item = gtk_ui_manager_get_widget(ui_manager, "/ui/context-menu/PasteSpecial");
        if (!menu_item) {
            menu_item = gtk_menu_item_new_with_label("Paste remote image link");
            g_signal_connect(menu_item, "activate", G_CALLBACK(paste_special_cb), composer);
            g_signal_connect(popup, "map", G_CALLBACK(update_menu_item_sensitivity), menu_item);

            gtk_menu_shell_append(GTK_MENU_SHELL(popup), menu_item);
            gtk_widget_show(menu_item);
        }
    }
}
static void m_msg_composer_extension_remote_images_constructed (GObject *object)
{
	EExtension *extension;
	EExtensible *extensible;

	/* Chain up to parent's method. */
    G_OBJECT_CLASS (m_msg_composer_extension_remote_images_parent_class)->constructed (object);

	extension = E_EXTENSION (object);
	extensible = e_extension_get_extensible (extension);

    m_msg_composer_extension_add_paste_action(M_MSG_COMPOSER_EXTENSION (object), E_MSG_COMPOSER (extensible));
}

static void m_msg_composer_extension_remote_images_class_init (MMsgComposerExtensionRemoteImagesClass *class)
{
	GObjectClass *object_class;
	EExtensionClass *extension_class;

	object_class = G_OBJECT_CLASS (class);
    object_class->constructed = m_msg_composer_extension_remote_images_constructed;

	/* Set the type to extend, it's supposed to implement the EExtensible interface */
	extension_class = E_EXTENSION_CLASS (class);
	extension_class->extensible_type = E_TYPE_MSG_COMPOSER;
}

static void m_msg_composer_extension_remote_images_class_finalize (MMsgComposerExtensionRemoteImagesClass * /*class*/)
{
}

static void m_msg_composer_extension_remote_images_init (MMsgComposerExtensionRemoteImages *msg_composer_ext)
{
    msg_composer_ext->priv = m_msg_composer_extension_remote_images_get_instance_private (msg_composer_ext);
}

void m_msg_composer_extension_remote_images_type_register (GTypeModule *type_module)
{
    m_msg_composer_extension_remote_images_register_type (type_module);
}

GtkWidget *e_plugin_lib_get_configure_widget (EPlugin * /*epl*/)
{
	GtkWidget *vbox;

	fprintf(stderr,"***** e_plugin_lib_get_configure_widget called!\n");

	vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);

	gtk_widget_show_all (vbox);

	return vbox;
}

/* Module Entry Points */
void e_module_load (GTypeModule *type_module);
void e_module_unload (GTypeModule *type_module);

	G_MODULE_EXPORT void
e_module_load (GTypeModule *type_module)
{
    m_msg_composer_extension_remote_images_type_register (type_module);
}

    G_MODULE_EXPORT void
e_module_unload (GTypeModule *type_module)
{
}
