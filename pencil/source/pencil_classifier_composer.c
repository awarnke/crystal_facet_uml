/* File: pencil_classifier_composer.c; Copyright and License: see below */

#include "pencil_classifier_composer.h"
#include "trace.h"
#include "util/string/utf8stringbuf.h"
#include "util/string/utf8string.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void pencil_classifier_composer_init( pencil_classifier_composer_t *this_ )
{
    TRACE_BEGIN();

    pencil_marker_init( &((*this_).marker) );
    data_rules_init ( &((*this_).data_rules) );
    draw_classifier_icon_init( &((*this_).draw_classifier_icon) );
    draw_classifier_label_init( &((*this_).draw_classifier_label) );
    draw_classifier_contour_init( &((*this_).draw_classifier_contour) );

    TRACE_END();
}

void pencil_classifier_composer_destroy( pencil_classifier_composer_t *this_ )
{
    TRACE_BEGIN();

    draw_classifier_icon_destroy( &((*this_).draw_classifier_icon) );
    draw_classifier_label_destroy( &((*this_).draw_classifier_label) );
    draw_classifier_contour_destroy( &((*this_).draw_classifier_contour) );
    data_rules_destroy ( &((*this_).data_rules) );
    pencil_marker_destroy( &((*this_).marker) );

    TRACE_END();
}

void pencil_classifier_composer_draw ( const pencil_classifier_composer_t *this_,
                                       const layout_visible_classifier_t *layouted_classifier,
                                       data_id_t mark_focused,
                                       data_id_t mark_highlighted,
                                       const data_small_set_t *mark_selected,
                                       const pencil_layout_data_t *layout_data,
                                       const pencil_size_t *pencil_size,
                                       PangoLayout *font_layout,
                                       cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != layouted_classifier );
    assert( NULL != layout_data );
    assert( NULL != font_layout );
    assert( NULL != cr );

    const data_visible_classifier_t *const visible_classifier
        = layout_visible_classifier_get_data_const( layouted_classifier );
    const geometry_rectangle_t *const classifier_symbol_box
        = layout_visible_classifier_get_symbol_box_const( layouted_classifier );
    const data_classifier_t *const classifier
        = data_visible_classifier_get_classifier_const( visible_classifier );
    const data_diagramelement_t *const diagramelement
        = data_visible_classifier_get_diagramelement_const( visible_classifier );
    const data_classifier_type_t classifier_type = data_classifier_get_main_type( classifier );

    const double gap = pencil_size_get_standard_object_border( pencil_size );

    /* draw id */
    draw_classifier_label_draw_id( &((*this_).draw_classifier_label),
                                   visible_classifier,
                                   classifier_symbol_box,
                                   pencil_size,
                                   font_layout,
                                   cr
                                 );

    /* draw the classifier */
    {
        data_diagramelement_flag_t display_flags;
        display_flags = data_diagramelement_get_display_flags( diagramelement );

        TRACE_INFO_INT("drawing classifier id", data_classifier_get_row_id( classifier ) );

        double std_line_width = pencil_size_get_standard_line_width( pencil_size );
        cairo_set_line_width( cr, std_line_width );

        /* set color */
        GdkRGBA foreground_color;
        {
            if ( data_id_equals_id( &mark_highlighted, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_row_id( diagramelement ) ) )
            {
                foreground_color = pencil_size_get_highlight_color( pencil_size );
            }
            else if ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_GRAY_OUT ))
            {
                foreground_color = pencil_size_get_gray_out_color( pencil_size );
            }
            else
            {
                foreground_color = pencil_size_get_standard_color( pencil_size );
            }
            cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
        }

        /* highlight */
        if ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_EMPHASIS ))
        {
            const GdkRGBA emph_color = pencil_size_get_emphasized_bgcolor( pencil_size );
            cairo_set_source_rgba( cr, emph_color.red, emph_color.green, emph_color.blue, emph_color.alpha );
            const geometry_rectangle_t *const box = layout_visible_classifier_get_label_box_const( layouted_classifier );
            cairo_rectangle ( cr,
                              geometry_rectangle_get_left(box),
                              geometry_rectangle_get_top(box),
                              geometry_rectangle_get_width(box),
                              geometry_rectangle_get_height(box)
                            );
            cairo_fill (cr);
            cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
        }

        /* draw label */
        draw_classifier_label_draw_stereotype_and_name( &((*this_).draw_classifier_label),
                                                        visible_classifier,
                                                        layout_visible_classifier_get_label_box_const( layouted_classifier ),
                                                        pencil_size,
                                                        font_layout,
                                                        cr
                                                      );

        /* draw rectangle */
        geometry_rectangle_t classifier_icon_box;
        {
            geometry_rectangle_copy( &classifier_icon_box, classifier_symbol_box );
            geometry_rectangle_enlarge( &classifier_icon_box, -2.0*gap, -2.0*gap );
            geometry_rectangle_shift( &classifier_icon_box, gap, gap );
        }
        const double icon_left = geometry_rectangle_get_left ( &classifier_icon_box );
        const double icon_top = geometry_rectangle_get_top ( &classifier_icon_box );
        const double icon_width = geometry_rectangle_get_width ( &classifier_icon_box );
        const double icon_height = geometry_rectangle_get_height ( &classifier_icon_box );
        switch ( classifier_type )
        {
            case DATA_CLASSIFIER_TYPE_REQUIREMENT:  /* SysML */
            case DATA_CLASSIFIER_TYPE_PART:
            {
                draw_classifier_contour_draw_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_BLOCK:  /* SysML */
            case DATA_CLASSIFIER_TYPE_CLASS:
            case DATA_CLASSIFIER_TYPE_OBJECT:
            case DATA_CLASSIFIER_TYPE_INTERFACE:
            {
                draw_classifier_contour_draw_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
                pencil_classifier_composer_private_draw_feature_compartments( this_,
                                                                              layouted_classifier,
                                                                              layout_data,
                                                                              pencil_size,
                                                                              cr
                                                                            );
            }
            break;

            case DATA_CLASSIFIER_TYPE_COMPONENT:
            {
                draw_classifier_contour_draw_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );

                /* draw icon */
                const double component_icon_height = pencil_size_get_title_font_size( pencil_size );
                const geometry_rectangle_t icon_bounds
                    = draw_classifier_icon_get_component_bounds( &((*this_).draw_classifier_icon),
                                                                 icon_left + icon_width - gap,  /* x */
                                                                 icon_top + gap,  /* y */
                                                                 GEOMETRY_H_ALIGN_RIGHT,
                                                                 GEOMETRY_V_ALIGN_TOP,
                                                                 component_icon_height
                                                               );
                draw_classifier_icon_draw_component ( &((*this_).draw_classifier_icon), icon_bounds, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_ARTIFACT:
            {
                draw_classifier_contour_draw_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );

                /* draw icon */
                const double artifact_icon_height = pencil_size_get_title_font_size( pencil_size );
                const geometry_rectangle_t icon_bounds
                    = draw_classifier_icon_get_artifact_bounds( &((*this_).draw_classifier_icon),
                                                                icon_left + icon_width - gap,  /* x */
                                                                icon_top + gap,  /* y */
                                                                GEOMETRY_H_ALIGN_RIGHT,
                                                                GEOMETRY_V_ALIGN_TOP,
                                                                artifact_icon_height
                                                              );
                draw_classifier_icon_draw_artifact ( &((*this_).draw_classifier_icon), icon_bounds, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_ACTIVITY:
            case DATA_CLASSIFIER_TYPE_STATE:
            case DATA_CLASSIFIER_TYPE_CONSTRAINT_BLOCK:
            {
                draw_classifier_contour_draw_rounded_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, false, pencil_size, cr );
                pencil_classifier_composer_private_draw_feature_compartments ( this_,
                                                                               layouted_classifier,
                                                                               layout_data,
                                                                               pencil_size,
                                                                               cr
                                                                             );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION:
            {
                draw_classifier_contour_draw_rounded_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, true, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_USE_CASE:
            {
                draw_classifier_contour_draw_ellipse ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
                pencil_classifier_composer_private_draw_feature_compartments ( this_,
                                                                               layouted_classifier,
                                                                               layout_data,
                                                                               pencil_size,
                                                                               cr
                                                                             );
            }
            break;

            case DATA_CLASSIFIER_TYPE_NODE:
            {
                draw_classifier_contour_draw_3d_box ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_ACTOR:
            {
                const double actor_height = icon_height;
                const double half_width = 0.5 * icon_width;
                const geometry_rectangle_t icon_bounds
                    = draw_classifier_icon_get_actor_bounds( &((*this_).draw_classifier_icon),
                                                             icon_left + half_width,
                                                             icon_top,
                                                             GEOMETRY_H_ALIGN_CENTER,
                                                             GEOMETRY_V_ALIGN_TOP,
                                                             actor_height
                                                           );
                draw_classifier_icon_draw_actor ( &((*this_).draw_classifier_icon), icon_bounds, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE:
            case DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE:
            case DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY:
            case DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY:
            {
                const double circle_diameter = icon_height;
                const double half_width = 0.5 * icon_width;
                const geometry_rectangle_t icon_bounds
                    = draw_classifier_icon_get_circle_bounds( &((*this_).draw_classifier_icon),
                                                              icon_left + half_width,
                                                              icon_top,
                                                              GEOMETRY_H_ALIGN_CENTER,
                                                              GEOMETRY_V_ALIGN_TOP,
                                                              circle_diameter
                                                            );
                const bool stroke = ( classifier_type != DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE );
                const bool fill = ( ( classifier_type == DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE )
                                  || ( classifier_type == DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE ) );
                const bool shallow_history = ( classifier_type == DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY );
                const bool deep_history = ( classifier_type == DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY );
                draw_classifier_icon_draw_circle( &((*this_).draw_classifier_icon),
                                                  icon_bounds,
                                                  pencil_size,
                                                  stroke,
                                                  fill,
                                                  shallow_history,
                                                  deep_history,
                                                  cr
                                                );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT :
            {
                draw_classifier_contour_draw_accept_event ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL:
            {
                draw_classifier_contour_draw_send_signal ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT:
            {
                const double time_icon_height = icon_height;
                const double half_width = 0.5 * icon_width;
                const geometry_rectangle_t icon_bounds
                    = draw_classifier_icon_get_time_bounds( &((*this_).draw_classifier_icon),
                                                            icon_left + half_width,
                                                            icon_top,
                                                            GEOMETRY_H_ALIGN_CENTER,
                                                            GEOMETRY_V_ALIGN_TOP,
                                                            time_icon_height
                                                          );
                draw_classifier_icon_draw_time ( &((*this_).draw_classifier_icon), icon_bounds, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_FORK_NODE:
            case DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE:
            {
                const double box_icon_height = icon_height;
                const double half_width = 0.5 * icon_width;
                const geometry_rectangle_t icon_bounds
                    = draw_classifier_icon_get_sync_bounds( &((*this_).draw_classifier_icon),
                                                            icon_left + half_width,
                                                            icon_top,
                                                            GEOMETRY_H_ALIGN_CENTER,
                                                            GEOMETRY_V_ALIGN_TOP,
                                                            box_icon_height,
                                                            pencil_size
                                                          );
                draw_classifier_icon_draw_sync ( &((*this_).draw_classifier_icon), icon_bounds, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE:
            {
                draw_classifier_contour_draw_rhombus ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_SUBSYSTEM:
            {
                draw_classifier_contour_draw_rect ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_DIAGRAM_REFERENCE:
            {
                draw_classifier_contour_draw_diagram_ref ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            case DATA_CLASSIFIER_TYPE_PACKAGE:
            {
                const geometry_rectangle_t *const classifier_label_box
                    = layout_visible_classifier_get_label_box_const( layouted_classifier );
                draw_classifier_contour_draw_package( &((*this_).draw_classifier_contour),
                                                      classifier_symbol_box,
                                                      classifier_label_box,
                                                      pencil_size,
                                                      cr
                                                    );
            }
            break;

            case DATA_CLASSIFIER_TYPE_COMMENT:
            {
                draw_classifier_contour_draw_comment ( &((*this_).draw_classifier_contour), classifier_symbol_box, pencil_size, cr );
            }
            break;

            default:
            {
                TSLOG_ERROR("unknown data_classifier_type_t in pencil_classifier_composer_draw()");
            }
            break;
        }

#ifdef PENCIL_LAYOUT_DATA_DRAW_FOR_DEBUG
        /* draw the rectangles */
        {
            const geometry_rectangle_t *const classifier_space
                = layout_visible_classifier_get_space_const( layouted_classifier );
            const geometry_rectangle_t *const classifier_label_box
                = layout_visible_classifier_get_label_box_const( layouted_classifier );

            cairo_set_source_rgba( cr, 1.0, 0.5, 0.6, 0.5 );
            cairo_rectangle ( cr,
                              geometry_rectangle_get_left ( classifier_symbol_box ),
                              geometry_rectangle_get_top ( classifier_symbol_box ),
                              geometry_rectangle_get_width ( classifier_symbol_box ),
                              geometry_rectangle_get_height ( classifier_symbol_box )
                            );
            cairo_rectangle ( cr,
                              geometry_rectangle_get_left ( classifier_space ),
                              geometry_rectangle_get_top ( classifier_space ),
                              geometry_rectangle_get_width ( classifier_space ),
                              geometry_rectangle_get_height ( classifier_space )
                            );
            cairo_rectangle ( cr,
                              geometry_rectangle_get_left ( classifier_label_box ),
                              geometry_rectangle_get_top ( classifier_label_box ),
                              geometry_rectangle_get_width ( classifier_label_box ),
                              geometry_rectangle_get_height ( classifier_label_box )
                            );
            cairo_stroke (cr);
        }
#endif

        if ( data_small_set_contains_row_id( mark_selected, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_row_id(diagramelement) ) )
        {
            pencil_marker_mark_selected_rectangle( &((*this_).marker), *classifier_symbol_box, cr );
        }

        if ( data_id_equals_id( &mark_focused, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_row_id(diagramelement) )
            ||  data_id_equals_id( &mark_focused, DATA_TABLE_CLASSIFIER, data_classifier_get_row_id(classifier) ) )
        {
            pencil_marker_mark_focused_rectangle( &((*this_).marker), *classifier_symbol_box, cr );
        }
    }

    TRACE_END();
}

static inline double MAX_OF_2( double a, double b ) { return ((a>b)?a:b); }
static inline double MAX_OF_3( double a, double b, double c ) { return ((a>b)?((a>c)?a:c):((b>c)?b:c)); }

void pencil_classifier_composer_set_all_bounds ( const pencil_classifier_composer_t *this_,
                                                 const geometry_dimensions_t *proposed_bounds,
                                                 const geometry_dimensions_t *minimum_feature_space,
                                                 bool shows_contained_children,
                                                 const pencil_size_t *pencil_size,
                                                 PangoLayout *font_layout,
                                                 layout_visible_classifier_t *io_classifier_layout )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != proposed_bounds );
    assert( NULL != minimum_feature_space );
    assert( NULL != io_classifier_layout );

    /* get data that shall be layouted/composed */
    const data_visible_classifier_t *const visible_classifier
        = layout_visible_classifier_get_data_const( io_classifier_layout );

    /* get classifier type */
    const data_classifier_t *const classifier
        = data_visible_classifier_get_classifier_const( visible_classifier );
    const data_classifier_type_t classifier_type
        = data_classifier_get_main_type( classifier );

    /* get standard gap size */
    const double gap = pencil_size_get_standard_object_border( pencil_size );

    /* get the symbol and label boxes and inner space rectangles to modify */
    geometry_rectangle_t *const out_classifier_symbol_box
        = layout_visible_classifier_get_symbol_box_ptr( io_classifier_layout );
    geometry_rectangle_t *const out_classifier_space
        = layout_visible_classifier_get_space_ptr( io_classifier_layout );
    geometry_rectangle_t *const out_classifier_label_box
        = layout_visible_classifier_get_label_box_ptr( io_classifier_layout );

    TRACE_INFO_INT("calculating bounds of classifier id", data_classifier_get_row_id( classifier ) );

    /* TODO the following call is somehow wrong; it does not provide proposed_bounds */
    /* or minimum_feature_space to the draw_classifier_contour object */
    /* - or it should return proportional sizes like space_width=67% */

    /* determine border sizes of the classifier-shape */
    double top_border;
    double left_border;
    double bottom_border;
    double right_border;
    draw_classifier_contour_get_shape_border_dimensions( &((*this_).draw_classifier_contour),
                                                         classifier_type,
                                                         pencil_size,
                                                         &top_border,
                                                         &left_border,
                                                         &bottom_border,
                                                         &right_border
                                                       );

    /* TODO handle exeptions of decision, use case, send signal, receive event */

    const geometry_dimensions_t icon_dim
        = draw_classifier_icon_get_icon_dimensions( &((*this_).draw_classifier_icon),
                                                    classifier_type,
                                                    pencil_size
                                                  );

    /* determine available label-space for current classifier type */


    /* determine stereotype and name dimensions */
    double text_width;
    double text_height;
    draw_classifier_label_get_stereotype_and_name_dimensions( &((*this_).draw_classifier_label),
                                                              visible_classifier,
                                                              proposed_bounds,
                                                              pencil_size,
                                                              font_layout,
                                                              &text_width,
                                                              &text_height
                                                            );

    const bool is_fix_sized_symbol
         = layout_visible_classifier_is_fix_sized_symbol( io_classifier_layout );

    /* adapt to the size of the contained features */
    if ( ! is_fix_sized_symbol )
    {
        const double icon_gap = ( geometry_dimensions_get_width( &icon_dim ) < 0.000001 ) ? 0.0 : gap;
        const double width_by_text_and_icon
            = left_border + text_width + icon_gap + geometry_dimensions_get_width( &icon_dim ) + right_border;
        const double width_by_space
            = left_border + geometry_dimensions_get_width( minimum_feature_space ) + right_border;
        const double width_by_proposal
            = geometry_dimensions_get_width( proposed_bounds );
        const double width
            = MAX_OF_3 ( width_by_text_and_icon, width_by_space, width_by_proposal );

        const double height_by_text_and_space = top_border + text_height + geometry_dimensions_get_height( minimum_feature_space ) + bottom_border;
        const double height_by_proposal = geometry_dimensions_get_height( proposed_bounds );
        const double height = MAX_OF_2 ( height_by_text_and_space, height_by_proposal );

        geometry_rectangle_reinit( out_classifier_symbol_box, 0.0, 0.0, width, height );

        /* update the borders based on the text dimensions */
        if ( DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE == classifier_type )
        {
            const geometry_v_align_t DECISION_V_ALIGN = GEOMETRY_V_ALIGN_CENTER;
            top_border = geometry_v_align_get_top( &DECISION_V_ALIGN,
                                                   text_height,
                                                   0.0,
                                                   height
                                                 );
            bottom_border = top_border;
        }

        /* calculate label_box */
        const bool is_package_with_contents = (classifier_type == DATA_CLASSIFIER_TYPE_PACKAGE) && shows_contained_children;
        const geometry_h_align_t text_h_align = is_package_with_contents ? GEOMETRY_H_ALIGN_LEFT : GEOMETRY_H_ALIGN_CENTER;
        const double text_left = geometry_h_align_get_left( &text_h_align,
                                                            text_width,
                                                            left_border,
                                                            width - left_border - icon_gap - geometry_dimensions_get_width( &icon_dim ) - right_border
                                                          );
        const double text_top = is_package_with_contents ? (bottom_border/*==2*gap*/) : (top_border);
        geometry_rectangle_reinit( out_classifier_label_box, text_left, text_top, text_width, text_height );
        layout_visible_classifier_set_label_anchor( io_classifier_layout,
                                                    GEOMETRY_H_ALIGN_CENTER,
                                                    GEOMETRY_V_ALIGN_TOP
                                                  );

        /* calculate space */
        const double space_width = width - left_border - right_border;
        const double space_height = height-top_border-text_height-bottom_border;
        geometry_rectangle_reinit( out_classifier_space, left_border, top_border+text_height, space_width, space_height );
    }
    else
    {
        const double symbol_height = pencil_size_get_classifier_symbol_height( pencil_size );
        const double symbol_width = symbol_height;

        /* calculate bounds */
        geometry_rectangle_reinit( out_classifier_symbol_box, 0.0, 0.0, symbol_width, symbol_height );

        /* calculate label_box */
        geometry_rectangle_reinit( out_classifier_label_box, (symbol_width-text_width)/2.0, symbol_height, text_width, text_height );
        layout_visible_classifier_set_label_anchor( io_classifier_layout,
                                                    GEOMETRY_H_ALIGN_CENTER,
                                                    GEOMETRY_V_ALIGN_TOP
                                                  );

        /* calculate space */
        geometry_rectangle_reinit( out_classifier_space, 0.0, symbol_height+text_height, symbol_width, 0.0 );
    }

    TRACE_END();
}

void pencil_classifier_composer_expand_inner_space ( const pencil_classifier_composer_t *this_,
                                                     const geometry_rectangle_t *inner_space,
                                                     bool shows_contained_children,
                                                     const pencil_size_t *pencil_size,
                                                     PangoLayout *font_layout,
                                                     layout_visible_classifier_t *io_classifier_layout )
{
    TRACE_BEGIN();
    assert( NULL != inner_space );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != io_classifier_layout );

    /* get data that shall be layouted/composed */
    const data_visible_classifier_t *const visible_classifier
        = layout_visible_classifier_get_data_const( io_classifier_layout );
    const data_classifier_t *const classifier
        = data_visible_classifier_get_classifier_const( visible_classifier );
    const data_classifier_type_t classifier_type
        = data_classifier_get_main_type( classifier );

    TRACE_INFO_INT("calculating bounds of classifier id", data_classifier_get_row_id( classifier ) );

    /* get the symbol and label boxes and inner space rectangles to modify */
    geometry_rectangle_t *const out_classifier_symbol_box
        = layout_visible_classifier_get_symbol_box_ptr( io_classifier_layout );
    geometry_rectangle_t *const out_classifier_space
        = layout_visible_classifier_get_space_ptr( io_classifier_layout );
    geometry_rectangle_t *const out_classifier_label_box
        = layout_visible_classifier_get_label_box_ptr( io_classifier_layout );

    /* get standard gap size */
    const double gap = pencil_size_get_standard_object_border( pencil_size );

    /* determine icon space */
    const geometry_dimensions_t icon_dim
        = draw_classifier_icon_get_icon_dimensions( &((*this_).draw_classifier_icon),
                                                    classifier_type,
                                                    pencil_size
                                                  );

/* TODO    TODO    TODO    TODO    TODO    TODO    TODO    TODO    TODO */


    /* shift to requested position */
    const geometry_rectangle_t *const current_space
        = layout_visible_classifier_get_space_const( io_classifier_layout );
    const double shift_to_right = geometry_rectangle_get_left( inner_space ) - geometry_rectangle_get_left( current_space );
    const double shift_to_bottom = geometry_rectangle_get_top( inner_space ) - geometry_rectangle_get_top( current_space );
    layout_visible_classifier_shift( io_classifier_layout, shift_to_right, shift_to_bottom );

    TRACE_END();
}

void pencil_classifier_composer_expand_envelope_box( const pencil_classifier_composer_t *this_,
                                                     const geometry_rectangle_t *envelope,
                                                     bool shows_contained_children,
                                                     const pencil_size_t *pencil_size,
                                                     PangoLayout *font_layout,
                                                     layout_visible_classifier_t *io_classifier_layout )
{
    TRACE_BEGIN();
    assert( NULL != envelope );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != io_classifier_layout );

    /* get data that shall be layouted/composed */
    const data_visible_classifier_t *const visible_classifier
        = layout_visible_classifier_get_data_const( io_classifier_layout );
    const data_classifier_t *const classifier
        = data_visible_classifier_get_classifier_const( visible_classifier );
    const data_classifier_type_t classifier_type
        = data_classifier_get_main_type( classifier );

    TRACE_INFO_INT("calculating bounds of classifier id", data_classifier_get_row_id( classifier ) );

    /* get standard gap size */
    const double gap = pencil_size_get_standard_object_border( pencil_size );

    /* determine icon space */
    const geometry_dimensions_t icon_dim
        = draw_classifier_icon_get_icon_dimensions( &((*this_).draw_classifier_icon),
                                                    classifier_type,
                                                    pencil_size
                                                  );

    /* determine border sizes of the classifier-shape */
    const geometry_rectangle_t inner_area
        = draw_classifier_contour_calc_inner_area( &((*this_).draw_classifier_contour),
                                                   classifier_type,
                                                   envelope,
                                                   pencil_size
                                                 );

    /* determine border sizes of the main line (label and optionally icon) */
    geometry_rectangle_t label_rect;
    geometry_rectangle_init_empty( &label_rect );
    const int area_too_small
        = pencil_classifier_composer_private_get_label_box( this_,
                                                            visible_classifier,
                                                            shows_contained_children,
                                                            &inner_area,
                                                            &icon_dim,
                                                            pencil_size,
                                                            font_layout,
                                                            &label_rect
                                                          );

    /* if label fits into inner_area */
    if ( 0 == area_too_small )
    {
        const bool is_fix_sized_symbol
            = layout_visible_classifier_is_fix_sized_symbol( io_classifier_layout );

        /* get the symbol and label boxes and inner space rectangles to modify */
        geometry_rectangle_t *const out_classifier_symbol_box
            = layout_visible_classifier_get_symbol_box_ptr( io_classifier_layout );
        geometry_rectangle_t *const out_classifier_space
            = layout_visible_classifier_get_space_ptr( io_classifier_layout );
        geometry_rectangle_t *const out_classifier_label_box
            = layout_visible_classifier_get_label_box_ptr( io_classifier_layout );

        if ( is_fix_sized_symbol )
        {
            const double symbol_height = pencil_size_get_classifier_symbol_height( pencil_size );
            const double symbol_width = symbol_height;

            /* calculate symbol bounds */
            const geometry_h_align_t H_CENTER = GEOMETRY_H_ALIGN_CENTER;
            const double symbol_left
                = geometry_h_align_get_left( &H_CENTER,
                                             symbol_width,
                                             geometry_rectangle_get_left( &inner_area ),
                                             geometry_rectangle_get_width( &inner_area )
                                           );
            geometry_rectangle_reinit( out_classifier_symbol_box, symbol_left, 0.0, symbol_width, symbol_height );
        }
        else
        {
            /* calculate symbol bounds */
            geometry_rectangle_replace( out_classifier_symbol_box, envelope );
        }

        /* calculate label_box */
        geometry_rectangle_replace( out_classifier_label_box, &label_rect );
        layout_visible_classifier_set_label_anchor( io_classifier_layout,
                                                    GEOMETRY_H_ALIGN_CENTER,
                                                    GEOMETRY_V_ALIGN_TOP
                                                  );

        /* calculate space */
        geometry_rectangle_replace( out_classifier_space, &inner_area );
        geometry_rectangle_shift( out_classifier_space, 0.0, geometry_rectangle_get_height( &label_rect ) );
        geometry_rectangle_enlarge( out_classifier_space, 0.0, -geometry_rectangle_get_height( &label_rect ) );
    }
    else
    {
        geometry_rectangle_t inner_space;
        geometry_rectangle_copy( &inner_space, &inner_area );
        geometry_rectangle_shift( &inner_space, 0.0, geometry_rectangle_get_height( &label_rect ) );
        geometry_rectangle_enlarge( &inner_space, 0.0, -geometry_rectangle_get_height( &label_rect ) );
        pencil_classifier_composer_expand_inner_space( this_,
                                                       &inner_space,
                                                       shows_contained_children,
                                                       pencil_size,
                                                       font_layout,
                                                       io_classifier_layout
                                                     );
        geometry_rectangle_destroy( &inner_space );

        /* shift to requested position */
        const geometry_rectangle_t current_envelope
            = layout_visible_classifier_calc_envelope_box( io_classifier_layout );
        const double shift_to_right = geometry_rectangle_get_left( envelope ) - geometry_rectangle_get_left( &current_envelope );
        const double shift_to_bottom = geometry_rectangle_get_top( envelope ) - geometry_rectangle_get_top( &current_envelope );
        layout_visible_classifier_shift( io_classifier_layout, shift_to_right, shift_to_bottom );
    }
    geometry_rectangle_destroy( &label_rect );

    TRACE_END();
}

int pencil_classifier_composer_private_get_label_box ( const pencil_classifier_composer_t *this_,
                                                       const data_visible_classifier_t *visible_classifier,
                                                       bool shows_contained_children,
                                                       const geometry_rectangle_t *inner_area_box,
                                                       const geometry_dimensions_t *icon_dim,
                                                       const pencil_size_t *pencil_size,
                                                       PangoLayout *font_layout,
                                                       geometry_rectangle_t *out_label_box )
{
    TRACE_BEGIN();
    assert( NULL != visible_classifier );
    assert( NULL != inner_area_box );
    assert( NULL != icon_dim );
    assert( NULL != pencil_size );
    assert( NULL != font_layout );
    assert( NULL != out_label_box );

    int result = 0;

#if 0
    /* get classifier type */
    const data_classifier_t *const classifier
        = data_visible_classifier_get_classifier_const( visible_classifier );
    const data_classifier_type_t classifier_type
        = data_classifier_get_main_type( classifier );

    /* get standard gap size */
    const double gap = pencil_size_get_standard_object_border( pencil_size );

    /* determine stereotype and name dimensions */
    const bool is_fix_sized_symbol
         = layout_visible_classifier_is_fix_sized_symbol( io_classifier_layout );
    const double icon_gap = is_fix_sized_symbol ? 0.0 : ( geometry_dimensions_get_width( &icon_dim ) < 0.000001 ) ? 0.0 : gap;
    const double proposed_label_width
        = is_fix_sized_symbol
        ? geometry_dimensions_get_width( &inner_area )
        : geometry_dimensions_get_width( &inner_area ) - geometry_dimensions_get_width( &icon_dim ) - icon_gap;
    const geometry_dimensions_t proposed_label_dim
        = geometry_dimensions_init( proposed_label_width, geometry_dimensions_get_hight( &inner_area ) );
    double text_width;
    double text_height;
    draw_classifier_label_get_stereotype_and_name_dimensions( &((*this_).draw_classifier_label),
                                                              visible_classifier,
                                                              &inner_dim,
                                                              pencil_size,
                                                              font_layout,
                                                              &text_width,
                                                              &text_height
                                                            );

    /* recalculate outer bounds if sum does not fit to inner_dim */
    if ( ! is_fix_sized_symbol )
    {
        const double width_by_text_and_icon
            = text_width + icon_gap + geometry_dimensions_get_width( &icon_dim );
        const double width_by_space
            = geometry_dimensions_get_width( minimum_feature_space );41 0
        const double needed_inner_width
            = MAX_OF_2 ( width_by_text_and_icon, width_by_space );

        const double needed_inner_height = text_height + geometry_dimensions_get_height( minimum_feature_space );




        geometry_rectangle_reinit( out_classifier_symbol_box, 0.0, 0.0, width, height );

        /* update the borders based on the text dimensions */
        if ( DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE == classifier_type )
        {
            const geometry_v_align_t DECISION_V_ALIGN = GEOMETRY_V_ALIGN_CENTER;
            top_border = geometry_v_align_get_top( &DECISION_V_ALIGN,
                                                   text_height,
                                                   0.0,
                                                   height
                                                 );
            bottom_border = top_border;
        }

        /* calculate label_box */
        const bool is_package_with_contents = (classifier_type == DATA_CLASSIFIER_TYPE_PACKAGE) && shows_contained_children;
        const geometry_h_align_t text_h_align = is_package_with_contents ? GEOMETRY_H_ALIGN_LEFT : GEOMETRY_H_ALIGN_CENTER;
        const double text_left = geometry_h_align_get_left( &text_h_align,
                                                            text_width,
                                                            left_border,
                                                            width - left_border - icon_gap - geometry_dimensions_get_width( &icon_dim ) - right_border
                                                          );
        const double text_top = is_package_with_contents ? (bottom_border/*==2*gap*/) : (top_border);
        geometry_rectangle_reinit( out_classifier_label_box, text_left, text_top, text_width, text_height );
        layout_visible_classifier_set_label_anchor( io_classifier_layout,
                                                    GEOMETRY_H_ALIGN_CENTER,
                                                    GEOMETRY_V_ALIGN_TOP
                                                  );

        /* calculate space */
        const double space_width = width - left_border - right_border;
        const double space_height = height-top_border-text_height-bottom_border;
        geometry_rectangle_reinit( out_classifier_space, left_border, top_border+text_height, space_width, space_height );
    }
    else
    {
        const double symbol_height = pencil_size_get_classifier_symbol_height( pencil_size );
        const double symbol_width = symbol_height;

        /* calculate bounds */
        geometry_rectangle_reinit( out_classifier_symbol_box, 0.0, 0.0, symbol_width, symbol_height );

        /* calculate label_box */
        geometry_rectangle_reinit( out_classifier_label_box, (symbol_width-text_width)/2.0, symbol_height, text_width, text_height );
        layout_visible_classifier_set_label_anchor( io_classifier_layout,
                                                    GEOMETRY_H_ALIGN_CENTER,
                                                    GEOMETRY_V_ALIGN_TOP
                                                  );

        /* calculate space */
        geometry_rectangle_reinit( out_classifier_space, 0.0, symbol_height+text_height, symbol_width, 0.0 );
    }

    geometry_rectangle_destroy( &outer_bounds );
#endif

    TRACE_END_ERR( result );
    return result;
}

void pencil_classifier_composer_private_draw_feature_compartments ( const pencil_classifier_composer_t *this_,
                                                                    const layout_visible_classifier_t *layouted_classifier,
                                                                    const pencil_layout_data_t *layout_data,
                                                                    const pencil_size_t *pencil_size,
                                                                    cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != layouted_classifier );
    assert( NULL != layout_data );
    assert( NULL != pencil_size );
    assert( NULL != cr );

    /* determine number of properties and operations */
    uint32_t count_properties = 0;
    uint32_t count_operations = 0;
    {
        /* define names for input data */
        const data_row_id_t diagele_id = layout_visible_classifier_get_diagramelement_id ( layouted_classifier );

        const uint32_t num_features = pencil_layout_data_get_feature_count ( layout_data );
        for ( uint32_t f_probe_idx = 0; f_probe_idx < num_features; f_probe_idx ++ )
        {
            const layout_feature_t *const f_probe_layout = pencil_layout_data_get_feature_const ( layout_data, f_probe_idx );
            assert ( NULL != f_probe_layout );
            const layout_visible_classifier_t *const probe_vis_classfy = layout_feature_get_classifier_const ( f_probe_layout );
            assert ( NULL != probe_vis_classfy );

            /* check if this f_probe_layout has the same diagram element id as the_feature */
            if ( diagele_id == layout_visible_classifier_get_diagramelement_id ( probe_vis_classfy ) )
            {
                /* this is a feature of the current layouted_classifier */
                /* define names for input data */
                const data_feature_t *const f_probe_data = layout_feature_get_data_const ( f_probe_layout );
                assert ( NULL != f_probe_data );
                const data_feature_type_t f_probe_type = data_feature_get_main_type ( f_probe_data );

                if ( DATA_FEATURE_TYPE_PROPERTY == f_probe_type )
                {
                    count_properties ++;
                }
                else if ( DATA_FEATURE_TYPE_OPERATION == f_probe_type )
                {
                    count_operations ++;
                }
            }
        }
    }

    /* draw compartments if there are features */
    if (( count_properties != 0 )||( count_operations != 0 ))
    {
        /* define names for input data */
        const geometry_rectangle_t *const classifier_symbol_box
            = layout_visible_classifier_get_symbol_box_const( layouted_classifier );
        const geometry_rectangle_t *const classifier_space
            = layout_visible_classifier_get_space_const( layouted_classifier );
        const double feature_height = pencil_size_get_standard_font_size( pencil_size )
            + pencil_size_get_font_line_gap( pencil_size );
        const double gap = pencil_size_get_standard_object_border( pencil_size );
        const double y_coordinate_1 = geometry_rectangle_get_top( classifier_space );
        const double y_coordinate_2 = geometry_rectangle_get_top( classifier_space )
            + ( count_properties * feature_height ) + ( 2.0 * gap );


        draw_classifier_contour_draw_compartment_line ( &((*this_).draw_classifier_contour),
                                                        classifier_symbol_box,
                                                        y_coordinate_1,
                                                        pencil_size,
                                                        cr
                                                      );
        draw_classifier_contour_draw_compartment_line ( &((*this_).draw_classifier_contour),
                                                        classifier_symbol_box,
                                                        y_coordinate_2,
                                                        pencil_size,
                                                        cr
                                                      );
        /*
        const double y_coordinate_3 = geometry_rectangle_get_top( classifier_space )
            + ( (count_properties+count_operations) * feature_height ) + ( 4.0 * gap );
        draw_classifier_contour_draw_compartment_line ( &((*this_).draw_classifier_contour),
                                                        classifier_symbol_box,
                                                        y_coordinate_3,
                                                        pencil_size,
                                                        cr
                                                      );
        */
    }

    TRACE_END();
}

/*
Copyright 2016-2021 Andreas Warnke
    http://www.apache.org/licenses/LICENSE-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at


Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
