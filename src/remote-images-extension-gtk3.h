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

#ifndef M_MSG_COMPOSER_EXTENSION_H
#define M_MSG_COMPOSER_EXTENSION_H

#include <glib-object.h>
#include <libebackend/libebackend.h>
#include <gtk/gtk.h>

/* Standard GObject macros */
#define M_TYPE_MSG_COMPOSER_EXTENSION \
    (m_msg_composer_extension_remote_images_get_type ())
#define M_MSG_COMPOSER_EXTENSION(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST \
    ((obj), M_TYPE_MSG_COMPOSER_EXTENSION, MMsgComposerExtensionRemoteImages))
#define M_MSG_COMPOSER_EXTENSION_CLASS(cls) \
	(G_TYPE_CHECK_CLASS_CAST \
    ((cls), M_TYPE_MSG_COMPOSER_EXTENSION, MMsgComposerExtensionRemoteImagesClass))
#define M_IS_MSG_COMPOSER_EXTENSION(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE \
	((obj), M_TYPE_MSG_COMPOSER_EXTENSION))
#define M_IS_MSG_COMPOSER_EXTENSION_CLASS(cls) \
	(G_TYPE_CHECK_CLASS_TYPE \
	((cls), M_TYPE_MSG_COMPOSER_EXTENSION))
#define M_MSG_COMPOSER_EXTENSION_GET_CLASS(obj) \
	(G_TYPE_INSTANCE_GET_CLASS \
    ((obj), M_TYPE_MSG_COMPOSER_EXTENSION, MMsgComposerExtensionRemoteImagesClass))

G_BEGIN_DECLS

typedef struct _MMsgComposerExtensionRemoteImages MMsgComposerExtensionRemoteImages;
typedef struct _MMsgComposerExtensionRemoteImagesClass MMsgComposerExtensionRemoteImagesClass;
typedef struct _MMsgComposerExtensionRemoteImagesPrivate MMsgComposerExtensionRemoteImagesPrivate;

struct _MMsgComposerExtensionRemoteImages
{
	EExtension parent;

    MMsgComposerExtensionRemoteImagesPrivate *priv;
};

struct _MMsgComposerExtensionRemoteImagesClass
{
	EExtensionClass parent;
};

GType	m_msg_composer_extension_remote_images_get_type	(void);
void	m_msg_composer_extension_remote_images_type_register	(GTypeModule *type_module);

G_END_DECLS

#endif /* M_MSG_COMPOSER_EXTENSION_H */
