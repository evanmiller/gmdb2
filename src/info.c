/* MDB Tools - A library for reading MS Access database file
 * Copyright (C) 2000 Brian Bruns
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include "gmdb.h"

extern GtkWidget *app;
extern MdbHandle *mdb;

typedef struct GMdbInfoWindow {
	GtkWidget *window;
} GMdbInfoWindow;

GMdbInfoWindow *infow;

/* callbacks */
GtkWidget *
gmdb_info_new()
{
GtkWidget *propswin, *label;
GladeXML *propswin_xml;
gchar title[100];
gchar tmpstr[20];
gchar *filename, *filepath;
int i;
struct stat st;
char* version;

	/* load the interface */
	propswin_xml = glade_xml_new(GMDB_GLADEDIR "gmdb-props.glade", NULL, NULL);
	/* connect the signals in the interface */
	glade_xml_signal_autoconnect(propswin_xml);
	
	filepath = g_strdup(mdb->f->filename);
	for (i=strlen(filepath);i>0 && filepath[i-1]!='/';i--);
	filename=&filepath[i];
	
	propswin = glade_xml_get_widget (propswin_xml, "props_dialog");
	g_snprintf(title, 100, "%s Properties",filename);
	gtk_window_set_title(GTK_WINDOW(propswin), title);
		
	label = glade_xml_get_widget (propswin_xml, "props_filename");
	gtk_label_set_text(GTK_LABEL(label), filename);	
		
	label = glade_xml_get_widget (propswin_xml, "props_jetver");
	if (mdb->f->jet_version == MDB_VER_JET3)
		version = "3 (Access 97)";
	else if (mdb->f->jet_version == MDB_VER_JET4)
		version = "4 (Access 2000/XP/2003)";
	else if (mdb->f->jet_version == MDB_VER_ACCDB_2007)
		version = "ACE 12 (Access 2007)";
	else if (mdb->f->jet_version == MDB_VER_ACCDB_2010)
		version = "ACE 14 (Access 2010)";
	else
		version = "Unknown";
	gtk_label_set_text(GTK_LABEL(label), version);

	label = glade_xml_get_widget (propswin_xml, "props_encrypted");
	gtk_label_set_text(GTK_LABEL(label), mdb->f->db_key ? "Yes" : "No");	

	assert( fstat(mdb->f->fd, &st)!=-1 );
	sprintf(tmpstr, "%ld K", st.st_size/1024);
	label = glade_xml_get_widget (propswin_xml, "props_filesize");
	gtk_label_set_text(GTK_LABEL(label), tmpstr);	
		
	sprintf(tmpstr, "%ld", st.st_size / mdb->fmt->pg_size);
	label = glade_xml_get_widget (propswin_xml, "props_numpages");
	gtk_label_set_text(GTK_LABEL(label), tmpstr);	

	sprintf(tmpstr, "%d", mdb->num_catalog);
	label = glade_xml_get_widget (propswin_xml, "props_numobjs");
	gtk_label_set_text(GTK_LABEL(label), tmpstr);	

	g_free(filepath);

	return propswin;
}
