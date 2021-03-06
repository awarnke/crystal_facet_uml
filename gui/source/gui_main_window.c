/* File: gui_main_window.c; Copyright and License: see below */

#include "gui_main_window.h"
#include "trace.h"
#include "storage/data_change_notifier.h"
#include "meta/meta_info.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>

void gui_main_window_init ( gui_main_window_t *this_,
                            ctrl_controller_t *controller,
                            data_database_t *database,
                            data_database_reader_t *db_reader,
                            gui_resources_t *res,
                            observer_t *window_close_observer,
                            observer_t *window_open_observer )
{
    TRACE_BEGIN();

    /* init own attributes */
    (*this_).window_close_observer = window_close_observer;
    (*this_).window_open_observer = window_open_observer;
    (*this_).database = database;

    /* init window */

    (*this_).window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    const char *window_title;
    window_title = data_database_get_filename_ptr( database );
    gtk_window_set_title(GTK_WINDOW( (*this_).window ), ( window_title == NULL ) ? META_INFO_PROGRAM_NAME_STR : window_title );
    gtk_widget_set_size_request((*this_).window, 800, 400);
    gtk_window_set_default_size(GTK_WINDOW( (*this_).window ), 16*70, 9*70);

    /* init the message widgets */

    (*this_).message_text_label = gtk_label_new ("");
#if ((( GTK_MAJOR_VERSION == 3 ) && ( GTK_MINOR_VERSION >= 16 ))||( GTK_MAJOR_VERSION > 3 ))
    gtk_label_set_xalign (GTK_LABEL( (*this_).message_text_label ), 0.0 );
#else
    gtk_misc_set_alignment (GTK_MISC( (*this_).message_text_label ), 0.0, 0.0 );
#endif
    (*this_).message_icon_image = gtk_image_new_from_pixbuf ( gui_resources_get_crystal_facet_uml( res ) );
    gui_simple_message_to_user_init( &((*this_).message_to_user), (*this_).message_text_label, (*this_).message_icon_image, res );

    /* init the keyboard shortcuts */

    (*this_).keyboard_shortcut_group = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW( (*this_).window ), (*this_).keyboard_shortcut_group);

    /* init tools */

    (*this_).file_use_db_icon = gtk_image_new_from_pixbuf( gui_resources_get_file_use_db( res ));
    (*this_).file_use_db = gtk_tool_button_new( (*this_).file_use_db_icon, "Use_DB" );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).file_use_db), "Create/Use DB" );

    (*this_).file_export_icon = gtk_image_new_from_pixbuf( gui_resources_get_file_export( res ));
    (*this_).file_export = gtk_tool_button_new( (*this_).file_export_icon, "Export" );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).file_export), "Export" );

    (*this_).file_new_window_icon = gtk_image_new_from_pixbuf( gui_resources_get_file_new_window( res ));
    (*this_).file_new_window = gtk_tool_button_new( (*this_).file_new_window_icon, "New Window" );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).file_new_window), "New Window" );

    GSList *group;

    (*this_).tool_navigate_icon = gtk_image_new_from_pixbuf( gui_resources_get_tool_navigate( res ));
    (*this_).tool_navigate = gtk_radio_tool_button_new( NULL );
    gtk_tool_button_set_label ( GTK_TOOL_BUTTON((*this_).tool_navigate), "Navigate");
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).tool_navigate), (*this_).tool_navigate_icon);
    group = gtk_radio_tool_button_get_group (GTK_RADIO_TOOL_BUTTON ((*this_).tool_navigate));
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).tool_navigate), "Navigate" );

    (*this_).tool_edit_icon = gtk_image_new_from_pixbuf( gui_resources_get_tool_edit( res ));
    (*this_).tool_edit = gtk_radio_tool_button_new( group );
    gtk_tool_button_set_label ( GTK_TOOL_BUTTON((*this_).tool_edit), "Edit");
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).tool_edit), (*this_).tool_edit_icon);
    group = gtk_radio_tool_button_get_group (GTK_RADIO_TOOL_BUTTON ((*this_).tool_edit));
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).tool_edit), "Edit" );

    (*this_).tool_create_icon = gtk_image_new_from_pixbuf( gui_resources_get_tool_create( res ));
    (*this_).tool_create = gtk_radio_tool_button_new( group );
    gtk_tool_button_set_label ( GTK_TOOL_BUTTON((*this_).tool_create), "Create");
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).tool_create), (*this_).tool_create_icon);
    group = gtk_radio_tool_button_get_group (GTK_RADIO_TOOL_BUTTON ((*this_).tool_create));
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).tool_create), "Create" );

    (*this_).tool_search_icon = gtk_image_new_from_pixbuf( gui_resources_get_tool_search( res ));
    (*this_).tool_search = gtk_radio_tool_button_new( group );
    gtk_tool_button_set_label ( GTK_TOOL_BUTTON((*this_).tool_search), "Search");
    gtk_tool_button_set_icon_widget( GTK_TOOL_BUTTON((*this_).tool_search), (*this_).tool_search_icon);
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).tool_search), "Search" );

    (*this_).edit_cut_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_cut( res ));
    (*this_).edit_cut = gtk_tool_button_new( (*this_).edit_cut_icon, "Cut" );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_cut), "Cut (Ctrl-X)" );
    /*
    GDK_KEY_x + GDK_CONTROL_MASK handling see gui_sketch_area_key_press_callback();
    moved there to not interfere with text input fields
    */

    (*this_).edit_copy_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_copy( res ));
    (*this_).edit_copy = gtk_tool_button_new( (*this_).edit_copy_icon, "Copy" );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_copy), "Copy (Ctrl-C)" );
    /*
    GDK_KEY_c + GDK_CONTROL_MASK handling see gui_sketch_area_key_press_callback();
    moved there to not interfere with text input fields
    */

    (*this_).edit_paste_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_paste( res ));
    (*this_).edit_paste = gtk_tool_button_new( (*this_).edit_paste_icon, "Paste" );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_paste), "Paste (Ctrl-V)" );
    /*
    GK_KEY_v + GDK_CONTROL_MASK handling see gui_sketch_area_key_press_callback();
    moved there to not interfere with text input fields
    */

    (*this_).edit_delete_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_delete( res ));
    (*this_).edit_delete = gtk_tool_button_new( (*this_).edit_delete_icon, "Delete" );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_delete), "Delete (DEL)" );
    /*
    GDK_KEY_Delete handling see gui_sketch_area_key_press_callback();
    moved there to not interfere with text input fields
    */

    (*this_).edit_instantiate_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_instantiate( res ));
    (*this_).edit_instantiate = gtk_tool_button_new( (*this_).edit_instantiate_icon, "Instantiate" );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_instantiate), "Instantiate" );

    (*this_).edit_highlight_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_highlight( res ));
    (*this_).edit_highlight = gtk_tool_button_new( (*this_).edit_highlight_icon, "Highlight" );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_highlight), "Highlight" );

    (*this_).edit_reset_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_reset( res ));
    (*this_).edit_reset = gtk_tool_button_new( (*this_).edit_reset_icon, "Reset" );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_reset), "Reset Selection" );

    (*this_).edit_undo_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_undo( res ));
    (*this_).edit_undo = gtk_tool_button_new( (*this_).edit_undo_icon, "Undo" );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_undo), "Undo (Ctrl-Z)" );
    gtk_widget_add_accelerator ( GTK_WIDGET((*this_).edit_undo),
                                 "clicked",
                                 (*this_).keyboard_shortcut_group,
                                 GDK_KEY_z,
                                 GDK_CONTROL_MASK,
                                 GTK_ACCEL_VISIBLE
                               );

    (*this_).edit_redo_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_redo( res ));
    (*this_).edit_redo = gtk_tool_button_new( (*this_).edit_redo_icon, "Redo" );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_redo), "Redo (Ctrl-Y)" );
    gtk_widget_add_accelerator ( GTK_WIDGET((*this_).edit_redo),
                                 "clicked",
                                 (*this_).keyboard_shortcut_group,
                                 GDK_KEY_y,
                                 GDK_CONTROL_MASK,
                                 GTK_ACCEL_VISIBLE
                               );

    (*this_).tool_about_icon = gtk_image_new_from_pixbuf( gui_resources_get_crystal_facet_uml( res ));
    (*this_).tool_about = gtk_tool_button_new( (*this_).tool_about_icon, "About" );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).tool_about), "About" );

    (*this_).toolbar = gtk_toolbar_new();

    /* determine the current/main clipboard */
    GtkClipboard *current_clipboard;
    {
        GdkScreen *current_screen;
        current_screen = gtk_window_get_screen ( GTK_WINDOW( (*this_).window ) );
        GdkDisplay *current_display;
        current_display = gdk_screen_get_display ( current_screen );
        current_clipboard = gtk_clipboard_get_for_display ( current_display, GDK_NONE ); /* GDK_SELECTION_PRIMARY does not work */
    }

    gui_marked_set_init( &((*this_).marker_data) );
    gui_toolbox_init( &((*this_).tools_data),
                      (*this_).toolbar,
                      (*this_).tool_navigate,
                      (*this_).tool_edit,
                      (*this_).tool_create,
                      (*this_).tool_search,
                      current_clipboard,
                      &((*this_).marker_data),
                      &((*this_).message_to_user),
                      db_reader,
                      controller
                    );

    /* init sketch area */

    (*this_).sketcharea = gtk_drawing_area_new();
    gtk_widget_set_events( (*this_).sketcharea, GDK_LEAVE_NOTIFY_MASK
        | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK );
    gtk_widget_set_hexpand ( (*this_).sketcharea, TRUE);
    gtk_widget_set_vexpand ( (*this_).sketcharea, TRUE);
    gtk_widget_set_can_focus( GTK_WIDGET( (*this_).sketcharea ), TRUE );  /* this allows the text entry widgets to lose the focus */
    gtk_widget_set_size_request ( GTK_WIDGET( (*this_).sketcharea ), 600, 360);  /* set a minimum initial size */
    /*gtk_widget_set_focus_on_click( GTK_WIDGET( (*this_).sketcharea ), TRUE ); not yet existing: since GTK 3.2 */
    gui_sketch_area_init( &((*this_).sketcharea_data),
                          (*this_).sketcharea,
                          &((*this_).marker_data),
                          &((*this_).tools_data),
                          &((*this_).message_to_user),
                          res,
                          controller,
                          db_reader
                        );

    /* init attribute editor widgets */

    (*this_).attr_section_icon = gtk_image_new_from_pixbuf ( gui_resources_get_edit_attributes_sect( res ) );
    gtk_widget_set_halign( (*this_).attr_section_icon, GTK_ALIGN_START );
    (*this_).id_label = gtk_label_new ( "" );
    (*this_).name_label = gtk_label_new ( "Name:" );
    (*this_).description_label = gtk_label_new ( "Specification:" );
    (*this_).stereotype_label = gtk_label_new ( "Stereotype/Valuetype:" );
    (*this_).type_label = gtk_label_new ( "Type:" );
#if ((( GTK_MAJOR_VERSION == 3 ) && ( GTK_MINOR_VERSION >= 16 ))||( GTK_MAJOR_VERSION > 3 ))
    gtk_label_set_xalign (GTK_LABEL( (*this_).id_label ), 1.0 ); /* align right */
    gtk_label_set_xalign (GTK_LABEL( (*this_).name_label ), 0.0 );
    gtk_label_set_xalign (GTK_LABEL( (*this_).description_label ), 0.0 );
    gtk_label_set_xalign (GTK_LABEL( (*this_).stereotype_label ), 0.0 );
    gtk_label_set_xalign (GTK_LABEL( (*this_).type_label ), 0.0 );
#else
    gtk_misc_set_alignment (GTK_MISC( (*this_).id_label ), 1.0, 0.0 );
    gtk_misc_set_alignment (GTK_MISC( (*this_).name_label ), 0.0, 0.0 );
    gtk_misc_set_alignment (GTK_MISC( (*this_).description_label ), 0.0, 0.0 );
    gtk_misc_set_alignment (GTK_MISC( (*this_).stereotype_label ), 0.0, 0.0 );
    gtk_misc_set_alignment (GTK_MISC( (*this_).type_label ), 0.0, 0.0 );
#endif

    (*this_).name_entry = gtk_entry_new();

    (*this_).description_text_view = gtk_text_view_new ();
    gtk_text_view_set_wrap_mode ( GTK_TEXT_VIEW( (*this_).description_text_view ),
                                  GTK_WRAP_WORD_CHAR );
    gtk_container_set_border_width ( GTK_CONTAINER( (*this_).description_text_view ), 12 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).description_text_view ), true );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).description_text_view ), false );
    /* need own scroll window container - otherwise the gtk_grid will manage the text view */
    (*this_).description_scroll_win = gtk_scrolled_window_new (NULL, NULL);
    gtk_container_add (GTK_CONTAINER ( (*this_).description_scroll_win ), (*this_).description_text_view );
    gtk_scrolled_window_set_policy ( GTK_SCROLLED_WINDOW( (*this_).description_scroll_win ),
                                     GTK_POLICY_AUTOMATIC,
                                     GTK_POLICY_AUTOMATIC
                                   );

    (*this_).stereotype_entry = gtk_entry_new();

    (*this_).edit_commit_button = gtk_button_new();
    (*this_).edit_commit_icon = gtk_image_new_from_pixbuf( gui_resources_get_edit_commit( res ));
    gtk_button_set_image ( GTK_BUTTON((*this_).edit_commit_button), (*this_).edit_commit_icon );
    gtk_button_set_label ( GTK_BUTTON((*this_).edit_commit_button), NULL );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).edit_commit_button), "Commit (Ctrl-S)" );
    gtk_widget_add_accelerator ( GTK_WIDGET((*this_).edit_commit_button),
                                 "clicked",
                                 (*this_).keyboard_shortcut_group,
                                 GDK_KEY_s,
                                 GDK_CONTROL_MASK,
                                 GTK_ACCEL_VISIBLE
                               );

    (*this_).type_combo_box = gtk_combo_box_new();
    GtkCellRenderer *column1;
    column1 = gtk_cell_renderer_pixbuf_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT((*this_).type_combo_box), column1, /*expand:*/ FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT((*this_).type_combo_box), column1, "pixbuf", 2, NULL);
    GtkCellRenderer *column2;
    column2 = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT((*this_).type_combo_box), column2, /*expand:*/ TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT((*this_).type_combo_box), column2, "text", 1, NULL);

    (*this_).type_icon_grid = gtk_icon_view_new();
    gtk_widget_set_halign( (*this_).type_icon_grid, GTK_ALIGN_END );
    gtk_icon_view_set_tooltip_column( GTK_ICON_VIEW((*this_).type_icon_grid), 1 );
    gtk_icon_view_set_pixbuf_column( GTK_ICON_VIEW((*this_).type_icon_grid), 2 );
    gtk_icon_view_set_selection_mode( GTK_ICON_VIEW((*this_).type_icon_grid), GTK_SELECTION_NONE );
    gtk_icon_view_set_item_padding( GTK_ICON_VIEW((*this_).type_icon_grid), 2 );
    gtk_icon_view_set_margin( GTK_ICON_VIEW((*this_).type_icon_grid), 0 );
    gtk_icon_view_set_column_spacing( GTK_ICON_VIEW((*this_).type_icon_grid), 4 );
    gtk_icon_view_set_row_spacing( GTK_ICON_VIEW((*this_).type_icon_grid), 0 );
    gtk_icon_view_set_columns( GTK_ICON_VIEW((*this_).type_icon_grid), 7 );

    gui_attributes_editor_init( &((*this_).attributes_editor),
                                GTK_LABEL( (*this_).id_label ),
                                GTK_ENTRY( (*this_).name_entry ),
                                GTK_ENTRY( (*this_).stereotype_entry ),
                                GTK_COMBO_BOX( (*this_).type_combo_box ),
                                GTK_ICON_VIEW( (*this_).type_icon_grid ),
                                GTK_TEXT_VIEW( (*this_).description_text_view ),
                                GTK_BUTTON( (*this_).edit_commit_button ),
                                res,
                                controller,
                                db_reader,
                                database,
                                &((*this_).message_to_user)
                              );

    /* init search widgets */
    (*this_).search_label = gtk_label_new ( "Search:" );
#if ((( GTK_MAJOR_VERSION == 3 ) && ( GTK_MINOR_VERSION >= 16 ))||( GTK_MAJOR_VERSION > 3 ))
    gtk_label_set_xalign (GTK_LABEL( (*this_).search_label ), 1.0 ); /* align right */
    gtk_label_set_yalign (GTK_LABEL( (*this_).search_label ), 0.5 ); /* align middle-height */
#else
    gtk_misc_set_alignment (GTK_MISC( (*this_).search_label ), 1.0, 0.5 );
#endif
    (*this_).search_entry = gtk_entry_new();
    (*this_).search_button = gtk_button_new();
    (*this_).search_btn_icon = gtk_image_new_from_pixbuf( gui_resources_get_tool_search( res ));
    gtk_button_set_image ( GTK_BUTTON((*this_).search_button), (*this_).search_btn_icon );
    gtk_button_set_label ( GTK_BUTTON((*this_).search_button), NULL );
    gtk_widget_set_tooltip_text( GTK_WIDGET((*this_).search_button), "Search" );
    gui_search_runner_init( &((*this_).search_runner),
                            &((*this_).message_to_user),
                            db_reader,
                            database,
                            &((*this_).sketcharea_data)
                          );
    gui_search_request_init( &((*this_).search_request),
                             (*this_).search_label,
                             (*this_).search_entry,
                             (*this_).search_button,
                             &((*this_).search_runner)
                           );

    /* init the file choosers */

    gui_file_use_db_dialog_init ( &((*this_).file_use_db_dialog),
                                  controller,
                                  database,
                                  GTK_WINDOW( (*this_).window ),
                                  &((*this_).message_to_user)
                                );
    gui_file_export_dialog_init( &((*this_).file_export_dialog),
                                 database,
                                 db_reader,
                                 GTK_WINDOW( (*this_).window ),
                                 &((*this_).message_to_user)
                               );

    TRACE_INFO("GTK+ Widgets are created.");

    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).file_use_db,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).file_export,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).file_new_window,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).tool_search,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).tool_navigate,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).tool_edit,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).tool_create,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).edit_cut,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).edit_copy,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).edit_paste,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).edit_delete,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).edit_instantiate,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).edit_highlight,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).edit_reset,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).edit_undo,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).edit_redo,-1);
    gtk_toolbar_insert ( GTK_TOOLBAR((*this_).toolbar),(*this_).tool_about,-1);

    (*this_).layout = gtk_grid_new();
    gtk_grid_set_column_homogeneous ( GTK_GRID((*this_).layout), false );  /* if true, the window would get resized */
    gtk_grid_set_row_homogeneous ( GTK_GRID((*this_).layout), false );
    /* parameter info: gtk_grid_attach (GtkGrid *grid, GtkWidget *child, gint left, gint top, gint width, gint height); */
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).toolbar, 0, 0, 4, 1 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).toolbar ), false );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).toolbar ), false );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).search_label, 0, 1, 1, 1 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).search_label ), false );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).search_label ), false );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).search_entry, 1, 1, 1, 1 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).search_entry ), false );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).search_entry ), true );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).search_button, 2, 1, 1, 1 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).search_button ), false );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).search_button ), false );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).sketcharea, 0, 2, 3, 12 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).sketcharea ), true );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).sketcharea ), true );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).attr_section_icon, 3, 2, 1, 1 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).id_label ), false );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).id_label ), false );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).id_label, 3, 3, 1, 1 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).id_label ), false );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).id_label ), false );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).name_label, 3, 4, 1, 1 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).name_label ), false );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).name_label ), false );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).name_entry, 3, 5, 1, 1 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).name_entry ), false );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).name_entry ), false );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).stereotype_label, 3, 6, 1, 1 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).stereotype_label ), false );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).stereotype_label ), false );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).stereotype_entry, 3, 7, 1, 1 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).stereotype_entry ), false );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).stereotype_entry ), false );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).type_label, 3, 8, 1, 1 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).type_label ), false );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).type_label ), false );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).type_combo_box, 3, 9, 1, 1 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).type_combo_box ), false );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).type_combo_box ), false );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).type_icon_grid, 3, 10, 1, 1 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).type_icon_grid ), false );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).type_icon_grid ), false );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).description_label, 3, 11, 1, 1 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).description_label ), false );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).description_label ), false );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).description_scroll_win, 3, 12, 1, 1 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).description_scroll_win ), true );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).description_scroll_win ), false );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).edit_commit_button, 3, 13, 1, 1 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).edit_commit_button ), false );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).edit_commit_button ), false );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).message_icon_image, 0, 14, 1, 1 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).message_icon_image ), false );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).message_icon_image ), false );
    gtk_grid_attach( GTK_GRID((*this_).layout), (*this_).message_text_label, 1, 14, 3, 1 );
    gtk_widget_set_vexpand ( GTK_WIDGET( (*this_).message_text_label ), false );
    gtk_widget_set_hexpand ( GTK_WIDGET( (*this_).message_text_label ), false );
    gtk_container_add(GTK_CONTAINER((*this_).window), (*this_).layout);

    TRACE_INFO("GTK+ Widgets are added to containers.");

    /* inject dependencies by signals */

    /* parameter info: g_signal_connect( instance-that-emits-the-signal, signal-name, callback-handler, data-to-be-passed-to-callback-handler) */
    g_signal_connect( G_OBJECT((*this_).window), "delete_event", G_CALLBACK(gui_main_window_delete_event_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).window), "destroy", G_CALLBACK(gui_main_window_destroy_event_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).window), DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME, G_CALLBACK(gui_main_window_data_changed_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).sketcharea), "draw", G_CALLBACK (gui_sketch_area_draw_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).sketcharea), "motion_notify_event", G_CALLBACK(gui_sketch_area_mouse_motion_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).sketcharea), "button_press_event", G_CALLBACK(gui_sketch_area_button_press_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).sketcharea), "button_release_event", G_CALLBACK(gui_sketch_area_button_release_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).sketcharea), "leave_notify_event", G_CALLBACK(gui_sketch_area_leave_notify_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).sketcharea), DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME, G_CALLBACK(gui_sketch_area_data_changed_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).sketcharea), "key_press_event", G_CALLBACK(gui_sketch_area_key_press_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).toolbar), GUI_TOOLBOX_GLIB_SIGNAL_NAME, G_CALLBACK(gui_sketch_area_tool_changed_callback), &((*this_).sketcharea_data) );
    g_signal_connect( G_OBJECT((*this_).file_use_db), "clicked", G_CALLBACK(gui_main_window_use_db_btn_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).file_export), "clicked", G_CALLBACK(gui_main_window_export_btn_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).file_new_window), "clicked", G_CALLBACK(gui_main_window_new_window_btn_callback), this_ );
    g_signal_connect( G_OBJECT((*this_).tool_navigate), "clicked", G_CALLBACK(gui_toolbox_navigate_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).tool_edit), "clicked", G_CALLBACK(gui_toolbox_edit_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).tool_create), "clicked", G_CALLBACK(gui_toolbox_create_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).tool_search), "clicked", G_CALLBACK(gui_toolbox_search_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).toolbar), GUI_TOOLBOX_GLIB_SIGNAL_NAME, G_CALLBACK(gui_search_request_tool_changed_callback), &((*this_).search_request) );
    g_signal_connect( G_OBJECT((*this_).search_entry), DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME, G_CALLBACK(gui_search_request_data_changed_callback), &((*this_).search_request) );
    g_signal_connect( G_OBJECT((*this_).search_entry), "activate", G_CALLBACK(gui_search_request_search_start_callback), &((*this_).search_request) );
    g_signal_connect( G_OBJECT((*this_).search_button), "clicked", G_CALLBACK(gui_search_request_search_start_callback), &((*this_).search_request) );
    g_signal_connect( G_OBJECT((*this_).edit_cut), "clicked", G_CALLBACK(gui_toolbox_cut_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_copy), "clicked", G_CALLBACK(gui_toolbox_copy_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_paste), "clicked", G_CALLBACK(gui_toolbox_paste_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_delete), "clicked", G_CALLBACK(gui_toolbox_delete_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_instantiate), "clicked", G_CALLBACK(gui_toolbox_instantiate_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_highlight), "clicked", G_CALLBACK(gui_toolbox_highlight_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_reset), "clicked", G_CALLBACK(gui_toolbox_reset_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_undo), "clicked", G_CALLBACK(gui_toolbox_undo_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).edit_redo), "clicked", G_CALLBACK(gui_toolbox_redo_btn_callback), &((*this_).tools_data) );
    g_signal_connect( G_OBJECT((*this_).name_entry), "focus-out-event", G_CALLBACK(gui_attributes_editor_name_focus_lost_callback), &((*this_).attributes_editor) );
    g_signal_connect( G_OBJECT((*this_).name_entry), "activate", G_CALLBACK(gui_attributes_editor_name_enter_callback), &((*this_).attributes_editor) );
    g_signal_connect( G_OBJECT((*this_).description_text_view), "focus-out-event", G_CALLBACK(gui_attributes_editor_description_focus_lost_callback), &((*this_).attributes_editor) );
    g_signal_connect( G_OBJECT((*this_).type_combo_box), "changed", G_CALLBACK(gui_attributes_editor_type_changed_callback), &((*this_).attributes_editor) );
    g_signal_connect( G_OBJECT((*this_).type_icon_grid), "item-activated", G_CALLBACK(gui_attributes_editor_type_shortlist_callback), &((*this_).attributes_editor) );
    g_signal_connect( G_OBJECT((*this_).stereotype_entry), "focus-out-event", G_CALLBACK(gui_attributes_editor_stereotype_focus_lost_callback), &((*this_).attributes_editor) );
    g_signal_connect( G_OBJECT((*this_).stereotype_entry), "activate", G_CALLBACK(gui_attributes_editor_stereotype_enter_callback), &((*this_).attributes_editor) );
    g_signal_connect( G_OBJECT((*this_).edit_commit_button), "clicked", G_CALLBACK(gui_attributes_editor_commit_clicked_callback), &((*this_).attributes_editor) );
    g_signal_connect( G_OBJECT((*this_).sketcharea), GUI_SKETCH_AREA_GLIB_SIGNAL_NAME, G_CALLBACK(gui_attributes_editor_selected_object_changed_callback), &((*this_).attributes_editor) );
    g_signal_connect( G_OBJECT((*this_).name_entry), DATA_CHANGE_NOTIFIER_GLIB_SIGNAL_NAME, G_CALLBACK(gui_attributes_editor_data_changed_callback), &((*this_).attributes_editor) );
        /* ^-- name is the  proxy for all widgets of attributes_editor */
    g_signal_connect( G_OBJECT((*this_).tool_about), "clicked", G_CALLBACK(gui_main_window_about_btn_callback), this_ );

    TRACE_INFO("GTK+ Callbacks are connected to widget events.");

    /* register observers */

    (*this_).data_notifier = data_database_get_notifier_ptr( database );
    data_change_notifier_add_listener( (*this_).data_notifier, G_OBJECT((*this_).window) );
    data_change_notifier_add_listener( (*this_).data_notifier, G_OBJECT((*this_).sketcharea) );
    data_change_notifier_add_listener( (*this_).data_notifier, G_OBJECT((*this_).name_entry) );
        /* ^-- name is the  proxy for all widgets of attributes_editor */
    data_change_notifier_add_listener( (*this_).data_notifier, G_OBJECT((*this_).search_entry) );

    TRACE_INFO("GTK+ Widgets are registered as listeners at signal emitter.");

    gtk_widget_show_all((*this_).window);
    gui_attributes_editor_update_widgets( &((*this_).attributes_editor) );  /* hide some widgets again */
#ifdef NDEBUG
    gui_simple_message_to_user_hide( &((*this_).message_to_user) );
#else
    gui_simple_message_to_user_show_message ( &((*this_).message_to_user), GUI_SIMPLE_MESSAGE_TYPE_INFO, GUI_SIMPLE_MESSAGE_CONTENT_DEBUG_MODE );
#endif
    gui_search_request_hide( &((*this_).search_request) );

    TRACE_INFO("GTK+ Widgets are shown.");
    TRACE_END();
}

void gui_main_window_destroy( gui_main_window_t *this_ )
{
    TRACE_BEGIN();

    data_change_notifier_remove_listener( (*this_).data_notifier, G_OBJECT((*this_).window) );
    data_change_notifier_remove_listener( (*this_).data_notifier, G_OBJECT((*this_).sketcharea) );
    data_change_notifier_remove_listener( (*this_).data_notifier, G_OBJECT((*this_).name_entry) );
    data_change_notifier_remove_listener( (*this_).data_notifier, G_OBJECT((*this_).search_entry) );

    TRACE_INFO("GTK+ Widgets are unregistered as listeners from data module.");

    gui_file_export_dialog_destroy ( &((*this_).file_export_dialog) );
    gui_file_use_db_dialog_destroy ( &((*this_).file_use_db_dialog) );

    TRACE_INFO("GTK+ hidden windows are destroyed.");

    gui_search_request_destroy( &((*this_).search_request) );
    gui_search_runner_destroy( &((*this_).search_runner) );
    gui_sketch_area_destroy( &((*this_).sketcharea_data) );
    gui_toolbox_destroy( &((*this_).tools_data) );
    gui_marked_set_destroy( &((*this_).marker_data) );
    gui_attributes_editor_destroy( &((*this_).attributes_editor) );
    gui_simple_message_to_user_destroy( &((*this_).message_to_user) );
    (*this_).database = NULL;

    TRACE_END();
}

void gui_main_window_destroy_event_callback( GtkWidget *widget, gpointer data )
{
    TRACE_BEGIN();
    gui_main_window_t *this_ = data;

    /* forward destroy request to gui_window_manager: */
    observer_notify( (*this_).window_close_observer, this_ );

    TRACE_TIMESTAMP();
    TRACE_END();
}

gboolean gui_main_window_delete_event_callback( GtkWidget *widget, GdkEvent *event, gpointer data )
{
    TRACE_BEGIN();

    TRACE_TIMESTAMP();
    TRACE_END();
    return false;  /* return false to trigger destroy event */
}

void gui_main_window_use_db_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_main_window_t *this_ = data;

    gui_simple_message_to_user_hide( &((*this_).message_to_user) );

    gui_file_use_db_dialog_show ( &((*this_).file_use_db_dialog) );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_main_window_export_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_main_window_t *this_ = data;

    gui_simple_message_to_user_hide( &((*this_).message_to_user) );

    gui_file_export_dialog_show ( &((*this_).file_export_dialog) );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_main_window_new_window_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_main_window_t *this_ = data;

    /* hide last message */
    gui_simple_message_to_user_hide( &((*this_).message_to_user) );

    /* forward new window request to gui_window_manager: */
    observer_notify( (*this_).window_open_observer, &((*this_).message_to_user) );

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_main_window_about_btn_callback( GtkWidget* button, gpointer data )
{
    TRACE_BEGIN();
    gui_main_window_t *this_ = data;

    if ( GUI_SIMPLE_MESSAGE_TYPE_ABOUT == gui_simple_message_to_user_get_type_id( &((*this_).message_to_user) ) )
    {
        /* hide the already visible about message */
        gui_simple_message_to_user_hide( &((*this_).message_to_user) );
    }
    else
    {
        gui_simple_message_to_user_show_message( &((*this_).message_to_user),
                                                 GUI_SIMPLE_MESSAGE_TYPE_ABOUT,
                                                 GUI_SIMPLE_MESSAGE_CONTENT_ABOUT
                                               );
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}

void gui_main_window_data_changed_callback( GtkWidget *window, data_change_message_t *msg, gpointer user_data )
{
    TRACE_BEGIN();
    gui_main_window_t *this_ = user_data;

    if (( DATA_CHANGE_EVENT_TYPE_DB_OPENED == data_change_message_get_event( msg ) )
        || ( DATA_CHANGE_EVENT_TYPE_DB_CLOSED == data_change_message_get_event( msg ) ))
    {
        /* the database has changed */
        const char *filename = data_database_get_filename_ptr( (*this_).database );
        if ( NULL == filename )
        {
            gtk_window_set_title(GTK_WINDOW((*this_).window), META_INFO_PROGRAM_NAME_STR );
        }
        else
        {
            gtk_window_set_title(GTK_WINDOW((*this_).window), filename );
        }
    }

    TRACE_TIMESTAMP();
    TRACE_END();
}


/*
Copyright 2016-2021 Andreas Warnke

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
