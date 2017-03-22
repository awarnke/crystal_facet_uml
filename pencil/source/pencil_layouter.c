/* File: pencil_layouter.c; Copyright and License: see below */

#include "pencil_layouter.h"
#include "trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void pencil_layouter_init( pencil_layouter_t *this_, pencil_input_data_t *input_data )
{
    TRACE_BEGIN();
    assert( NULL != input_data );

    geometry_rectangle_init_empty( &((*this_).diagram_bounds) );
    pencil_size_init_empty( &((*this_).pencil_size) );
    geometry_rectangle_init_empty( &((*this_).diagram_draw_area) );
    geometry_non_linear_scale_init( &((*this_).x_scale), 0.0, 1.0 );
    geometry_non_linear_scale_init( &((*this_).y_scale), 0.0, 1.0 );
    geometry_rectangle_init_empty( &((*this_).default_classifier_size) );

    pencil_input_data_layout_init( &((*this_).layout_data) );
    (*this_).input_data = input_data;

    pencil_diagram_painter_init( &((*this_).diagram_painter) );
    pencil_classifier_painter_init( &((*this_).classifier_painter) );
    pencil_feature_painter_init( &((*this_).feature_painter) );
    pencil_relationship_painter_init( &((*this_).relationship_painter) );

    TRACE_END();
}

void pencil_layouter_destroy( pencil_layouter_t *this_ )
{
    TRACE_BEGIN();

    pencil_diagram_painter_destroy( &((*this_).diagram_painter) );
    pencil_classifier_painter_destroy( &((*this_).classifier_painter) );
    pencil_feature_painter_destroy( &((*this_).feature_painter) );
    pencil_relationship_painter_destroy( &((*this_).relationship_painter) );

    geometry_rectangle_destroy( &((*this_).diagram_bounds) );
    pencil_size_destroy( &((*this_).pencil_size) );
    geometry_rectangle_destroy( &((*this_).diagram_draw_area) );
    geometry_non_linear_scale_destroy( &((*this_).x_scale) );
    geometry_non_linear_scale_destroy( &((*this_).y_scale) );
    geometry_rectangle_destroy( &((*this_).default_classifier_size) );

    pencil_input_data_layout_destroy( &((*this_).layout_data) );
    (*this_).input_data = NULL;

    TRACE_END();
}

void pencil_layouter_layout_grid ( pencil_layouter_t *this_,
                                   pencil_input_data_t *input_data,
                                   geometry_rectangle_t diagram_bounds )
{
    TRACE_BEGIN();
    assert( NULL != input_data );

    /* update the pointer to the input data */
    (*this_).input_data = input_data;

    /* update the bounding rectangle */
    geometry_rectangle_replace( &((*this_).diagram_bounds), &diagram_bounds );

    /* calculate the pencil-sizes and the drawing rectangle */
    double width = geometry_rectangle_get_width ( &((*this_).diagram_bounds) );
    double height = geometry_rectangle_get_height ( &((*this_).diagram_bounds) );
    pencil_size_reinit( &((*this_).pencil_size), width, height );

    pencil_diagram_painter_get_drawing_space ( &((*this_).diagram_painter),
                                               pencil_input_data_get_diagram_ptr ( input_data ),
                                               &((*this_).pencil_size),
                                               &((*this_).diagram_bounds),
                                               &((*this_).diagram_draw_area) );

    /* calculate the axis scales */
    double draw_left = geometry_rectangle_get_left ( &((*this_).diagram_draw_area) );
    double draw_top = geometry_rectangle_get_top ( &((*this_).diagram_draw_area) );
    double draw_right = geometry_rectangle_get_right ( &((*this_).diagram_draw_area) );
    double draw_bottom = geometry_rectangle_get_bottom ( &((*this_).diagram_draw_area) );
    geometry_non_linear_scale_reinit( &((*this_).x_scale), draw_left, draw_right );
    geometry_non_linear_scale_reinit( &((*this_).y_scale), draw_top, draw_bottom );

    /* iterate over all classifiers */
    uint32_t count;
    count = pencil_input_data_get_visible_classifier_count ( input_data );
    for ( uint32_t index = 0; index < count; index ++ )
    {
        data_visible_classifier_t *visible_classifier;
        visible_classifier = pencil_input_data_get_visible_classifier_ptr ( input_data, index );

        if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
        {
            data_classifier_t *classifier;
            classifier = data_visible_classifier_get_classifier_ptr( visible_classifier );

            /* adjust the non-linear scales for this classifier */
            geometry_non_linear_scale_add_order ( &((*this_).x_scale), data_classifier_get_x_order( classifier ) );
            geometry_non_linear_scale_add_order ( &((*this_).y_scale), data_classifier_get_y_order( classifier ) );
        }
    }

    TRACE_END();
}

void pencil_layouter_layout_elements ( pencil_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();

    /* adjust the default classifier rectangle */
    pencil_layouter_private_propose_default_classifier_size( this_ );

    /* store the classifier bounds into input_data_layouter_t */
    pencil_layouter_private_estimate_classifier_bounds( this_, font_layout );

    /* move the classifiers to avoid overlaps */
    pencil_layouter_private_move_classifiers_to_avoid_overlaps( this_ );

    /* calculate the relationship shapes */
    pencil_layouter_private_determine_relationship_shapes( this_ );

    TRACE_END();
}

void pencil_layouter_private_propose_default_classifier_size ( pencil_layouter_t *this_ )
{
    TRACE_BEGIN();

    /* adjust the default classifier rectangle */
    uint32_t count_clasfy;
    count_clasfy = pencil_input_data_get_visible_classifier_count ( (*this_).input_data );

    double diagram_area = geometry_rectangle_get_area( &((*this_).diagram_draw_area) );
    double classifier_area;
    if ( count_clasfy > 0 )
    {
        classifier_area = diagram_area / count_clasfy * (0.10);
    }
    else
    {
        classifier_area = diagram_area * (0.10);
    }
    double half_width = sqrt(classifier_area);
    double half_height = half_width / 2.1;
    geometry_rectangle_reinit( &((*this_).default_classifier_size), -half_width, -half_height, 2.0 * half_width, 2.0 * half_height );

    TRACE_END();
}

void pencil_layouter_private_estimate_classifier_bounds ( pencil_layouter_t *this_, PangoLayout *font_layout )
{
    TRACE_BEGIN();

    /* store the classifier bounds into input_data_layouter_t */
    uint32_t count_clasfy;
    count_clasfy = pencil_input_data_get_visible_classifier_count ( (*this_).input_data );
    for ( uint32_t index = 0; index < count_clasfy; index ++ )
    {
        data_visible_classifier_t *visible_classifier2;
        visible_classifier2 = pencil_input_data_get_visible_classifier_ptr ( (*this_).input_data, index );

        if (( visible_classifier2 != NULL ) && ( data_visible_classifier_is_valid( visible_classifier2 ) ))
        {
            data_classifier_t *classifier2;
            classifier2 = data_visible_classifier_get_classifier_ptr( visible_classifier2 );

            /* determine the minimum size of the classifier */
            geometry_rectangle_t minimum_size;
            pencil_classifier_painter_get_minimum_bounds ( &((*this_).classifier_painter),
                                                           visible_classifier2,
                                                           &((*this_).pencil_size ),
                                                           font_layout,
                                                           &minimum_size
                                                         );
            double min_width = geometry_rectangle_get_width( &minimum_size );
            double min_height = geometry_rectangle_get_height( &minimum_size );

            /* determine the minimum size of the contained features */
            {
                geometry_rectangle_t features_bounds;
                geometry_rectangle_init_empty( &features_bounds );
                pencil_layouter_private_calculate_features_bounds ( this_,
                                                                    data_classifier_get_id( classifier2 ),
                                                                    font_layout,
                                                                    &features_bounds
                );

                /* adjust width and height to feature bounds */
                double feat_width = geometry_rectangle_get_width( &features_bounds );
                feat_width += 4.0 * pencil_size_get_standard_object_border( &((*this_).pencil_size ) );
                min_width = ( min_width < feat_width ) ? feat_width : min_width;
                min_height += geometry_rectangle_get_height( &features_bounds );

                geometry_rectangle_destroy( &features_bounds );
            }

            /* determine default size */
            double width = geometry_rectangle_get_width( &((*this_).default_classifier_size) );
            double height = geometry_rectangle_get_height( &((*this_).default_classifier_size) );

            /* get the bounds rectangle to modify */
            geometry_rectangle_t *classifier_bounds;
            classifier_bounds = pencil_input_data_layout_get_classifier_bounds_ptr( &((*this_).layout_data), index );

            /* adjust width and height to minumum bounds */
            width = ( width < min_width ) ? min_width : width;
            height = ( height < min_height ) ? min_height : height;

            /* overwrite the bounds rectangle */
            int32_t order_x = data_classifier_get_x_order( classifier2 );
            int32_t order_y = data_classifier_get_y_order( classifier2 );
            double center_x = geometry_non_linear_scale_get_location( &((*this_).x_scale), order_x );
            double center_y = geometry_non_linear_scale_get_location( &((*this_).y_scale), order_y );
            geometry_rectangle_reinit( classifier_bounds, center_x-0.5*width, center_y-0.5*height, width, height );

            /* adjust the inner space rectangle to modify */
            geometry_rectangle_t *classifier_space;
            classifier_space = pencil_input_data_layout_get_classifier_space_ptr( &((*this_).layout_data), index );
            pencil_classifier_painter_get_drawing_space ( &((*this_).classifier_painter),
                                                          visible_classifier2,
                                                          &((*this_).pencil_size ),
                                                          classifier_bounds,
                                                          font_layout,
                                                          classifier_space
                                                        );
        }
    }

    TRACE_END();
}

void pencil_layouter_private_calculate_features_bounds ( pencil_layouter_t *this_,
                                                         int64_t classifier_id,
                                                         PangoLayout *font_layout,
                                                         geometry_rectangle_t *out_features_bounds )
{
    TRACE_BEGIN();
    assert( NULL != font_layout );
    assert( NULL != out_features_bounds );

    double left = 0.0;
    double top = 0.0;
    double width = 0.0;
    double height = 0.0;

    /* search all contained features */
    for ( uint32_t f_idx = 0; f_idx < pencil_input_data_get_feature_count ( (*this_).input_data ); f_idx ++ )
    {
        data_feature_t *the_feature;
        the_feature = pencil_input_data_get_feature_ptr ( (*this_).input_data, f_idx );
        if ( data_feature_is_valid( the_feature ) )
        {
            if ( data_feature_get_classifier_id( the_feature ) == classifier_id )
            {
                geometry_rectangle_t min_feature_bounds;
                geometry_rectangle_init_empty( &min_feature_bounds );
                pencil_feature_painter_get_minimum_bounds ( &((*this_).feature_painter),
                                                            the_feature,
                                                            &((*this_).pencil_size),
                                                            font_layout,
                                                            &min_feature_bounds
                                                        );

                double current_w = geometry_rectangle_get_width( &min_feature_bounds );
                width = ( width < current_w ) ? current_w : width;
                height += geometry_rectangle_get_height( &min_feature_bounds );

                geometry_rectangle_destroy( &min_feature_bounds );
            }
        }
        else
        {
            TSLOG_ERROR("invalid feature in array!");
        }
    }

    geometry_rectangle_reinit( out_features_bounds, left, top, width, height );
    TRACE_END();
}

void pencil_layouter_private_move_classifiers_to_avoid_overlaps ( pencil_layouter_t *this_ )
{
    TRACE_BEGIN();
    assert ( UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= PENCIL_INPUT_DATA_MAX_CLASSIFIERS );

    universal_array_index_sorter_t sorted;
    universal_array_index_sorter_init( &sorted );

    /* sort the classifiers by their movement-needs */
    pencil_layouter_private_propose_order_to_move_classifiers ( this_, &sorted );

    /* move the classifiers */
    uint32_t count_sorted;
    count_sorted = universal_array_index_sorter_get_count( &sorted );
    for ( uint32_t sort_index = 0; sort_index < count_sorted; sort_index ++ )
    {
        /* declaration of list of options */
        uint32_t solutions_count = 0;
        static const uint32_t SOLUTIONS_MAX = 5;
        double solution_move_dx[5];
        double solution_move_dy[5];

        /* propose options */
        pencil_layouter_private_propose_solutions_to_move_classifier ( this_,
                                                                       &sorted,
                                                                       sort_index,
                                                                       SOLUTIONS_MAX,
                                                                       solution_move_dx,
                                                                       solution_move_dy,
                                                                       &solutions_count
                                                                     );

        /* select best option */
        uint32_t index_of_best;
        if ( 1 == solutions_count )
        {
            index_of_best = 0;
        }
        else
        {
            pencil_layouter_private_select_solution_to_move_classifier ( this_,
                                                                        &sorted,
                                                                        sort_index,
                                                                        solutions_count,
                                                                        solution_move_dx,
                                                                        solution_move_dy,
                                                                        &index_of_best
                                                                    );
        }

        /* perform best option */
        uint32_t index;
        index = universal_array_index_sorter_get_array_index( &sorted, sort_index );
        /* move the classifier */
        geometry_rectangle_t *classifier_bounds;
        classifier_bounds = pencil_input_data_layout_get_classifier_bounds_ptr( &((*this_).layout_data), index );
        geometry_rectangle_shift ( classifier_bounds, solution_move_dx[index_of_best], solution_move_dy[index_of_best] );
        /* move all contained features */
        geometry_rectangle_t *classifier_space;
        classifier_space = pencil_input_data_layout_get_classifier_space_ptr( &((*this_).layout_data), index );
        geometry_rectangle_shift ( classifier_space, solution_move_dx[index_of_best], solution_move_dy[index_of_best] );
    }

    universal_array_index_sorter_destroy( &sorted );

    TRACE_END();
}

void pencil_layouter_private_propose_order_to_move_classifiers ( pencil_layouter_t *this_, universal_array_index_sorter_t *out_sorted )
{
    TRACE_BEGIN();
    assert ( NULL != out_sorted );
    assert ( UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= PENCIL_INPUT_DATA_MAX_CLASSIFIERS );

    /* sort the classifiers by their movement-needs */
    uint32_t count_clasfy;
    count_clasfy = pencil_input_data_get_visible_classifier_count ( (*this_).input_data );
    for ( uint32_t index = 0; index < count_clasfy; index ++ )
    {
        geometry_rectangle_t *classifier_bounds;
        classifier_bounds = pencil_input_data_layout_get_classifier_bounds_ptr( &((*this_).layout_data), index );

        int64_t weight = 0;

        /* reduce weight by area outside the diagram border */
        {
            geometry_rectangle_t border_intersect;
            int intersect_error2;
            intersect_error2 = geometry_rectangle_init_by_intersect( &border_intersect, classifier_bounds, &((*this_).diagram_draw_area) );
            if ( 0 != intersect_error2 )
            {
                TSLOG_WARNING( "a rectangle to be drawn is completely outside the diagram area" );
            }

            weight += 16.0 * geometry_rectangle_get_area( &border_intersect );
            weight -= 16.0 * geometry_rectangle_get_area( classifier_bounds );

            geometry_rectangle_destroy( &border_intersect );
        }

        /* reduce weight by intersects with other rectangles */
        for ( uint32_t probe_index = 0; probe_index < count_clasfy; probe_index ++ )
        {
            geometry_rectangle_t *probe_bounds;
            probe_bounds = pencil_input_data_layout_get_classifier_bounds_ptr( &((*this_).layout_data), probe_index );

            geometry_rectangle_t intersect;
            int intersect_error;
            intersect_error = geometry_rectangle_init_by_intersect( &intersect, classifier_bounds, probe_bounds );

            if ( 0 == intersect_error )
            {
                weight -= geometry_rectangle_get_area( &intersect );
            }

            geometry_rectangle_destroy( &intersect );
        }

        int insert_error;
        insert_error = universal_array_index_sorter_insert( out_sorted, index, weight );
        if ( 0 != insert_error )
        {
            TSLOG_WARNING( "not all rectangles are moved" );
        }
    }

    TRACE_END();
}

/*!
 *  \brief constants for directions of moving objects
 */
enum pencil_layouter_private_move_enum {
    PENCIL_LAYOUTER_PRIVATE_MOVE_NOT = 0,  /*!< only move to visible arey - nothing more */
    PENCIL_LAYOUTER_PRIVATE_MOVE_UP_MIN = 1,  /*!< moves up the minimum distance (up means smaller y-values) */
    PENCIL_LAYOUTER_PRIVATE_MOVE_DOWN_MIN = 2,
    PENCIL_LAYOUTER_PRIVATE_MOVE_LEFT_MIN = 3,
    PENCIL_LAYOUTER_PRIVATE_MOVE_RIGHT_MIN = 4,
    PENCIL_LAYOUTER_PRIVATE_MOVE_MAX = 5,  /*!< constant defining the total number of available options */
};

void pencil_layouter_private_propose_solutions_to_move_classifier ( pencil_layouter_t *this_,
                                                                    const universal_array_index_sorter_t *sorted,
                                                                    uint32_t sort_index,
                                                                    uint32_t solutions_max,
                                                                    double out_solution_move_dx[],
                                                                    double out_solution_move_dy[],
                                                                    uint32_t *out_solutions_count )
{
    TRACE_BEGIN();
    assert ( NULL != sorted );
    assert ( universal_array_index_sorter_get_count( sorted ) > sort_index );
    assert ( NULL != out_solution_move_dx );
    assert ( NULL != out_solution_move_dy );
    assert ( NULL != out_solutions_count );
    assert ( 1 <= solutions_max );  /* general requirement to report at least one option */
    assert ( PENCIL_LAYOUTER_PRIVATE_MOVE_MAX <= solutions_max );  /* current implementation requires at least 5 options */

    /* get classifier to move */
    uint32_t index;
    index = universal_array_index_sorter_get_array_index( sorted, sort_index );
    geometry_rectangle_t *classifier_bounds;
    classifier_bounds = pencil_input_data_layout_get_classifier_bounds_ptr( &((*this_).layout_data), index );
    double top;
    double bottom;
    double left;
    double right;
    top = geometry_rectangle_get_top ( classifier_bounds );
    bottom = geometry_rectangle_get_bottom ( classifier_bounds );
    left = geometry_rectangle_get_left ( classifier_bounds );
    right = geometry_rectangle_get_right ( classifier_bounds );

    /* choose distance */
    double shift_x = 0.0;
    double shift_y = 0.0;

    /* check overlap to diagram boundary */
    {
        if ( bottom > geometry_rectangle_get_bottom( &((*this_).diagram_draw_area) ) )
        {
            shift_y = geometry_rectangle_get_bottom( &((*this_).diagram_draw_area) ) - bottom;
        }
        if ( top < geometry_rectangle_get_top( &((*this_).diagram_draw_area) ) )
        {
            shift_y = geometry_rectangle_get_top( &((*this_).diagram_draw_area) ) - top;
        }
        if ( right > geometry_rectangle_get_right( &((*this_).diagram_draw_area) ) )
        {
            shift_x = geometry_rectangle_get_right( &((*this_).diagram_draw_area) ) - right;
        }
        if ( left < geometry_rectangle_get_left( &((*this_).diagram_draw_area) ) )
        {
            shift_x = geometry_rectangle_get_left( &((*this_).diagram_draw_area) ) - left;
        }
    }

    *out_solutions_count = 1;
    out_solution_move_dx[PENCIL_LAYOUTER_PRIVATE_MOVE_NOT] = shift_x;
    out_solution_move_dy[PENCIL_LAYOUTER_PRIVATE_MOVE_NOT] = shift_y;

    /* determine minimum and comfort distances between classifiers */
    pencil_size_t *my_pencil_size = &((*this_).pencil_size);
    double gap = pencil_size_get_standard_object_border( my_pencil_size );

    out_solution_move_dx[PENCIL_LAYOUTER_PRIVATE_MOVE_UP_MIN] = shift_x;
    out_solution_move_dy[PENCIL_LAYOUTER_PRIVATE_MOVE_UP_MIN] = shift_y;
    out_solution_move_dx[PENCIL_LAYOUTER_PRIVATE_MOVE_DOWN_MIN] = shift_x;
    out_solution_move_dy[PENCIL_LAYOUTER_PRIVATE_MOVE_DOWN_MIN] = shift_y;
    out_solution_move_dx[PENCIL_LAYOUTER_PRIVATE_MOVE_LEFT_MIN] = shift_x;
    out_solution_move_dy[PENCIL_LAYOUTER_PRIVATE_MOVE_LEFT_MIN] = shift_y;
    out_solution_move_dx[PENCIL_LAYOUTER_PRIVATE_MOVE_RIGHT_MIN] = shift_x;
    out_solution_move_dy[PENCIL_LAYOUTER_PRIVATE_MOVE_RIGHT_MIN] = shift_y;

    /* adjust information on current rectangle */
    top += shift_y;
    bottom += shift_y;
    left += shift_x;
    right += shift_x;

    /* check overlap to already moved classifiers */
    for ( uint32_t probe_sort_index = 0; probe_sort_index < sort_index; probe_sort_index ++ )
    {
        /* get classifier to check overlaps */
        uint32_t probe_index;
        probe_index = universal_array_index_sorter_get_array_index( sorted, probe_sort_index );
        geometry_rectangle_t *probe_bounds;
        probe_bounds = pencil_input_data_layout_get_classifier_bounds_ptr( &((*this_).layout_data), probe_index );
        double probe_top;
        double probe_bottom;
        double probe_left;
        double probe_right;
        probe_top = geometry_rectangle_get_top ( probe_bounds );
        probe_bottom = geometry_rectangle_get_bottom ( probe_bounds );
        probe_left = geometry_rectangle_get_left ( probe_bounds );
        probe_right = geometry_rectangle_get_right ( probe_bounds );

        if ( probe_right < left )
        {
            /* no overlap, finished. */
        }
        else if ( probe_left > right )
        {
            /* no overlap, finished. */
        }
        else if ( probe_bottom < top )
        {
            /* no overlap, finished. */
        }
        else if ( probe_top > bottom )
        {
            /* no overlap, finished. */
        }
        else
        {
            /* there is an overlap - at least when considering the comfort zone */

            double my_shift_x_left_min;
            my_shift_x_left_min = probe_left - right - gap;
            if ( my_shift_x_left_min < out_solution_move_dx[PENCIL_LAYOUTER_PRIVATE_MOVE_LEFT_MIN] )
            {
                out_solution_move_dx[PENCIL_LAYOUTER_PRIVATE_MOVE_LEFT_MIN] = my_shift_x_left_min;
            }

            double my_shift_x_right_min;
            my_shift_x_right_min = probe_right - left + gap;
            if ( my_shift_x_right_min > out_solution_move_dx[PENCIL_LAYOUTER_PRIVATE_MOVE_RIGHT_MIN] )
            {
                out_solution_move_dx[PENCIL_LAYOUTER_PRIVATE_MOVE_RIGHT_MIN] = my_shift_x_right_min;
            }

            double my_shift_y_up_min;
            my_shift_y_up_min = probe_top - bottom - gap;
            if ( my_shift_y_up_min < out_solution_move_dy[PENCIL_LAYOUTER_PRIVATE_MOVE_UP_MIN] )
            {
                out_solution_move_dy[PENCIL_LAYOUTER_PRIVATE_MOVE_UP_MIN] = my_shift_y_up_min;
            }

            double my_shift_y_down_min;
            my_shift_y_down_min = probe_bottom - top + gap;
            if ( my_shift_y_down_min > out_solution_move_dy[PENCIL_LAYOUTER_PRIVATE_MOVE_DOWN_MIN] )
            {
                out_solution_move_dy[PENCIL_LAYOUTER_PRIVATE_MOVE_DOWN_MIN] = my_shift_y_down_min;
            }

            *out_solutions_count = PENCIL_LAYOUTER_PRIVATE_MOVE_MAX;

            /* trace */
            data_visible_classifier_t *visible_classifier_p;
            visible_classifier_p = pencil_input_data_get_visible_classifier_ptr ( (*this_).input_data, probe_index );
            if (( visible_classifier_p != NULL ) && ( data_visible_classifier_is_valid( visible_classifier_p ) ))
            {
                data_classifier_t *classifier_p;
                classifier_p = data_visible_classifier_get_classifier_ptr( visible_classifier_p );
                TRACE_INFO_STR( "- overlaps:", data_classifier_get_name_ptr( classifier_p ) );
            }
        }
    }

    /* trace */
    data_visible_classifier_t *visible_classifier;
    visible_classifier = pencil_input_data_get_visible_classifier_ptr ( (*this_).input_data, index );
    if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
    {
        data_classifier_t *classifier;
        classifier = data_visible_classifier_get_classifier_ptr( visible_classifier );
        TRACE_INFO_STR( "classifier:", data_classifier_get_name_ptr( classifier ) );
    }

    TRACE_END();
}

void pencil_layouter_private_select_solution_to_move_classifier ( pencil_layouter_t *this_,
                                                                  const universal_array_index_sorter_t *sorted,
                                                                  uint32_t sort_index,
                                                                  uint32_t solutions_count,
                                                                  const double solution_move_dx[],
                                                                  const double solution_move_dy[],
                                                                  uint32_t *out_index_of_best )
{
    TRACE_BEGIN();
    assert ( NULL != sorted );
    assert ( universal_array_index_sorter_get_count( sorted ) > sort_index );
    assert ( NULL != solution_move_dx );
    assert ( NULL != solution_move_dy );
    assert ( NULL != out_index_of_best );
    assert ( 1 <= solutions_count );

    /* define potential solution and rating */
    uint32_t index_of_best;
    double debts_of_best;
    index_of_best = 0;  /* in case of doubts, take the first solution */
    debts_of_best = 1000000000.0;

    /* get classifier to move */
    uint32_t index;
    index = universal_array_index_sorter_get_array_index( sorted, sort_index );
    geometry_rectangle_t *classifier_bounds;
    classifier_bounds = pencil_input_data_layout_get_classifier_bounds_ptr( &((*this_).layout_data), index );

    /* check all solutions */
    for ( uint32_t solution_index = 0; solution_index < solutions_count; solution_index ++ )
    {
        /* calculate the solution rectangle */
        geometry_rectangle_t solution_bounds;
        geometry_rectangle_copy( &solution_bounds, classifier_bounds );
        geometry_rectangle_shift ( &solution_bounds, solution_move_dx[solution_index], solution_move_dy[solution_index] );

        /* evalute the debts of this solution */
        double debts_of_current;
        debts_of_current = 0.0;

        /* add move distance as debt */
        debts_of_current += fabs ( solution_move_dx[solution_index] );
        debts_of_current += fabs ( solution_move_dy[solution_index] );

        /* add debts for overlap to diagram boundary */
        {
            static const double DIAG_BOUNDS_SEVERITY = 32.0;

            double current_area = geometry_rectangle_get_area ( &solution_bounds );
            geometry_rectangle_t intersect;
            geometry_rectangle_init_by_intersect( &intersect, &solution_bounds, &((*this_).diagram_draw_area) );
            double intersect_area = geometry_rectangle_get_area ( &intersect );

            debts_of_current += DIAG_BOUNDS_SEVERITY * ( current_area - intersect_area );
        }

        /* check overlap to other classifiers */
        for ( uint32_t probe_sort_index = 0; probe_sort_index < universal_array_index_sorter_get_count( sorted ); probe_sort_index ++ )
        {
            if ( probe_sort_index != sort_index )  /* skip self */
            {
                /* get classifier to check overlaps */
                uint32_t probe_index;
                probe_index = universal_array_index_sorter_get_array_index( sorted, probe_sort_index );
                geometry_rectangle_t *probe_bounds;
                probe_bounds = pencil_input_data_layout_get_classifier_bounds_ptr( &((*this_).layout_data), probe_index );

                geometry_rectangle_t probe_intersect;
                int intersect_err;
                intersect_err = geometry_rectangle_init_by_intersect( &probe_intersect, &solution_bounds, probe_bounds );
                if ( 0 == intersect_err )
                {
                    /* already processed classifiers have higher severity because these do not move anymore */
                    double severity = ( probe_sort_index < sort_index ) ? 4.0 : 1.0;
                    double probe_intersect_area = geometry_rectangle_get_area ( &probe_intersect );
                    debts_of_current += severity * probe_intersect_area;
                }
                /* else no intersect/overlap */
            }
        }

        /* finish evaluating this solution */
        geometry_rectangle_destroy( &solution_bounds );
        if ( debts_of_current < debts_of_best )
        {
            debts_of_best = debts_of_current;
            index_of_best = solution_index;
        }
    }

    *out_index_of_best = index_of_best;

    TRACE_END();
}

void pencil_layouter_private_determine_relationship_shapes ( pencil_layouter_t *this_ )
{
    TRACE_BEGIN();
    assert ( UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= PENCIL_INPUT_DATA_MAX_RELATIONSHIPS );

    universal_array_index_sorter_t sorted;
    universal_array_index_sorter_init( &sorted );

    /* sort the relationships by their movement-needs, drop invisible relations */
    pencil_layouter_private_propose_order_to_shape_relationships ( this_, &sorted );

    /* shape the relationships */
    uint32_t count_sorted;
    count_sorted = universal_array_index_sorter_get_count( &sorted );
    for ( uint32_t sort_index = 0; sort_index < count_sorted; sort_index ++ )
    {
        /* declaration of list of options */
        uint32_t solutions_count = 0;
        static const uint32_t SOLUTIONS_MAX = 10;
        geometry_connector_t solution[10];

        /* propose options */
        pencil_layouter_private_propose_solutions_to_shape_relationship ( this_,
                                                                       &sorted,
                                                                       sort_index,
                                                                       SOLUTIONS_MAX,
                                                                       solution,
                                                                       &solutions_count
        );

        /* select best option */
        uint32_t index_of_best;
        if ( 1 == solutions_count )
        {
            index_of_best = 0;
        }
        else
        {
            pencil_layouter_private_select_solution_to_shape_relationship ( this_,
                                                                         &sorted,
                                                                         sort_index,
                                                                         solutions_count,
                                                                         solution,
                                                                         &index_of_best
            );
        }

        /* perform best option */
        uint32_t index;
        index = universal_array_index_sorter_get_array_index( &sorted, sort_index );
        /* copy the relationship shape */
        geometry_connector_t *relationship_shape;
        relationship_shape = pencil_input_data_layout_get_relationship_shape_ptr( &((*this_).layout_data), index );
        geometry_connector_copy( relationship_shape, &(solution[index_of_best]) );
    }

    universal_array_index_sorter_destroy( &sorted );

    TRACE_END();
}

void pencil_layouter_private_propose_order_to_shape_relationships ( pencil_layouter_t *this_, universal_array_index_sorter_t *out_sorted )
{
    TRACE_BEGIN();
    assert ( NULL != out_sorted );
    assert ( UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= PENCIL_INPUT_DATA_MAX_RELATIONSHIPS );

    /* sort the relationships by their shaping-needs */
    uint32_t count_relations;
    count_relations = pencil_input_data_get_relationship_count ( (*this_).input_data );
    for ( uint32_t index = 0; index < count_relations; index ++ )
    {
        data_relationship_t *current_relation;
        current_relation = pencil_input_data_get_relationship_ptr ( (*this_).input_data, index );

        int64_t weight = 0;

        /* determine weight by distance between source and destination */
        int32_t source_index;
        int32_t dest_index;
        source_index = pencil_input_data_get_classifier_index( (*this_).input_data,
                                                               data_relationship_get_from_classifier_id(current_relation)
                                                             );
        dest_index = pencil_input_data_get_classifier_index( (*this_).input_data,
                                                             data_relationship_get_to_classifier_id(current_relation)
                                                           );
        if (( -1 != source_index ) && ( -1 != dest_index ))
        {
            geometry_rectangle_t *source_rect;
            geometry_rectangle_t *dest_rect;
            source_rect = pencil_input_data_layout_get_classifier_bounds_ptr( &((*this_).layout_data), source_index );
            dest_rect = pencil_input_data_layout_get_classifier_bounds_ptr( &((*this_).layout_data), dest_index );
            weight -= fabs ( geometry_rectangle_get_x_center(source_rect) - geometry_rectangle_get_x_center(dest_rect) );
            weight -= fabs ( geometry_rectangle_get_y_center(source_rect) - geometry_rectangle_get_y_center(dest_rect) );

            int insert_error;
            insert_error = universal_array_index_sorter_insert( out_sorted, index, weight );
            if ( 0 != insert_error )
            {
                TSLOG_WARNING( "not all relationships are shaped" );
            }

            pencil_input_data_layout_set_relationship_visible ( &((*this_).layout_data), index, true );
        }
        else
        {
            TRACE_INFO_INT( "relationship not shown because one of the parties is not visible. index:", index );
            pencil_input_data_layout_set_relationship_visible ( &((*this_).layout_data), index, false );
        }
    }

    TRACE_END();
}

void pencil_layouter_private_propose_solutions_to_shape_relationship ( pencil_layouter_t *this_,
                                                                       const universal_array_index_sorter_t *sorted,
                                                                       uint32_t sort_index,
                                                                       uint32_t solutions_max,
                                                                       geometry_connector_t out_solutions[],
                                                                       uint32_t *out_solutions_count )
{
    TRACE_BEGIN();
    assert ( NULL != sorted );
    assert ( universal_array_index_sorter_get_count( sorted ) > sort_index );
    assert ( NULL != out_solutions );
    assert ( NULL != out_solutions_count );
    assert ( 1 <= solutions_max );  /* general requirement to report at least one option */
    assert ( 10 <= solutions_max );  /* current implementation requires at least 10 options */

    uint32_t index;
    index = universal_array_index_sorter_get_array_index( sorted, sort_index );

    data_relationship_t *current_relation;
    current_relation = pencil_input_data_get_relationship_ptr ( (*this_).input_data, index );

    /* propose connections between source and destination */
    int32_t source_index;
    int32_t dest_index;
    source_index = pencil_input_data_get_classifier_index( (*this_).input_data,
                                                           data_relationship_get_from_classifier_id(current_relation)
                                                         );
    dest_index = pencil_input_data_get_classifier_index( (*this_).input_data,
                                                         data_relationship_get_to_classifier_id(current_relation)
                                                       );
    if (( -1 != source_index ) && ( -1 != dest_index ))
    {
        geometry_rectangle_t *source_rect;
        geometry_rectangle_t *dest_rect;
        source_rect = pencil_input_data_layout_get_classifier_bounds_ptr( &((*this_).layout_data), source_index );
        dest_rect = pencil_input_data_layout_get_classifier_bounds_ptr( &((*this_).layout_data), dest_index );

        uint32_t solutions_by_ZN;
        uint32_t solutions_by_L7;
        uint32_t solutions_by_UC;

        pencil_layouter_private_connect_rectangles_by_ZN ( this_,
                                                           source_rect,
                                                           dest_rect,
                                                           solutions_max,
                                                           &(out_solutions[0]),
                                                           &solutions_by_ZN
                                                         );

        pencil_layouter_private_connect_rectangles_by_L7 ( this_,
                                                           source_rect,
                                                           dest_rect,
                                                           solutions_max - solutions_by_ZN,
                                                           &(out_solutions[solutions_by_ZN]),
                                                           &solutions_by_L7
                                                         );

        pencil_layouter_private_connect_rectangles_by_UC ( this_,
                                                           source_rect,
                                                           dest_rect,
                                                           solutions_max - solutions_by_ZN - solutions_by_L7,
                                                           &(out_solutions[solutions_by_ZN + solutions_by_L7]),
                                                           &solutions_by_UC
                                                         );

        *out_solutions_count = solutions_by_ZN + solutions_by_L7 + solutions_by_UC;
    }
    else
    {
        TSLOG_ERROR( "invisible relationship in sorted list." );
        *out_solutions_count = 0;
    }

    TRACE_END();
}

void pencil_layouter_private_select_solution_to_shape_relationship ( pencil_layouter_t *this_,
                                                                     const universal_array_index_sorter_t *sorted,
                                                                     uint32_t sort_index,
                                                                     uint32_t solutions_count,
                                                                     const geometry_connector_t solutions[],
                                                                     uint32_t *out_index_of_best )
{
    TRACE_BEGIN();
    assert ( NULL != sorted );
    assert ( universal_array_index_sorter_get_count( sorted ) > sort_index );
    assert ( NULL != solutions );
    assert ( NULL != out_index_of_best );
    assert ( 1 <= solutions_count );

    static unsigned int random;
    random ++;
    *out_index_of_best = random % solutions_count;

    /* the first is the best */
    /*
    *out_index_of_best = 0;
    */

    TRACE_END();
}

void pencil_layouter_private_connect_rectangles_by_ZN ( pencil_layouter_t *this_,
                                                        const geometry_rectangle_t *source_rect,
                                                        const geometry_rectangle_t *dest_rect,
                                                        uint32_t solutions_max,
                                                        geometry_connector_t out_solutions[],
                                                        uint32_t *out_solutions_count )
{
    TRACE_BEGIN();
    assert( NULL != source_rect );
    assert( NULL != dest_rect );
    assert ( NULL != out_solutions );
    assert ( NULL != out_solutions_count );
    assert ( 2 <= solutions_max );  /* current implementation requires at least 2 options */

    double x_dist;
    double y_dist;

    if ( geometry_rectangle_get_left(source_rect) > geometry_rectangle_get_right(dest_rect) )
    {
        /* source is right of destination, x_dist is negative */
        x_dist = geometry_rectangle_get_right(dest_rect) - geometry_rectangle_get_left(source_rect);
    }
    else if ( geometry_rectangle_get_right(source_rect) < geometry_rectangle_get_left(dest_rect) )
    {
        /* source is left of destination, x_dist is positive */
        x_dist = geometry_rectangle_get_left(dest_rect) - geometry_rectangle_get_right(source_rect);
    }
    else
    {
        /* overlap */
        x_dist = 0.0;
    }

    if ( geometry_rectangle_get_top(source_rect) > geometry_rectangle_get_bottom(dest_rect) )
    {
        /* source is below destination, y_dist is negative */
        y_dist = geometry_rectangle_get_bottom(dest_rect) - geometry_rectangle_get_top(source_rect);
    }
    else if ( geometry_rectangle_get_bottom(source_rect) < geometry_rectangle_get_top(dest_rect) )
    {
        /* source is atop of destination, y_dist is positive */
        y_dist = geometry_rectangle_get_top(dest_rect) - geometry_rectangle_get_bottom(source_rect);
    }
    else
    {
        /* overlap */
        y_dist = 0.0;
    }

    if ( fabs(x_dist) > fabs(y_dist) )
    {
        /* main line is vertical */
        if ( x_dist < 0.0 )
        {
            geometry_connector_reinit_vertical ( &(out_solutions[0]),
                                                 geometry_rectangle_get_left(source_rect),
                                                 geometry_rectangle_get_y_center(source_rect),
                                                 geometry_rectangle_get_right(dest_rect),
                                                 geometry_rectangle_get_y_center(dest_rect),
                                                 0.5*( geometry_rectangle_get_left(source_rect)+geometry_rectangle_get_right(dest_rect) )
                                               );
        }
        else
        {
            geometry_connector_reinit_vertical ( &(out_solutions[0]),
                                                 geometry_rectangle_get_right(source_rect),
                                                 geometry_rectangle_get_y_center(source_rect),
                                                 geometry_rectangle_get_left(dest_rect),
                                                 geometry_rectangle_get_y_center(dest_rect),
                                                 0.5*( geometry_rectangle_get_right(source_rect)+geometry_rectangle_get_left(dest_rect) )
                                               );
        }
    }
    else
    {
        /* main line is horizontal */
        if ( y_dist < 0.0 )
        {
            geometry_connector_reinit_horizontal ( &(out_solutions[0]),
                                                   geometry_rectangle_get_x_center(source_rect),
                                                   geometry_rectangle_get_top(source_rect),
                                                   geometry_rectangle_get_x_center(dest_rect),
                                                   geometry_rectangle_get_bottom(dest_rect),
                                                   0.5*( geometry_rectangle_get_top(source_rect)+geometry_rectangle_get_bottom(dest_rect) )
                                                 );
        }
        else
        {
            geometry_connector_reinit_horizontal ( &(out_solutions[0]),
                                                   geometry_rectangle_get_x_center(source_rect),
                                                   geometry_rectangle_get_bottom(source_rect),
                                                   geometry_rectangle_get_x_center(dest_rect),
                                                   geometry_rectangle_get_top(dest_rect),
                                                   0.5*( geometry_rectangle_get_bottom(source_rect)+geometry_rectangle_get_top(dest_rect) )
                                                 );
        }

    }

    *out_solutions_count = 1;

    TRACE_END();
}

void pencil_layouter_private_connect_rectangles_by_UC ( pencil_layouter_t *this_,
                                                        const geometry_rectangle_t *source_rect,
                                                        const geometry_rectangle_t *dest_rect,
                                                        uint32_t solutions_max,
                                                        geometry_connector_t out_solutions[],
                                                        uint32_t *out_solutions_count )
{
    TRACE_BEGIN();
    assert( NULL != source_rect );
    assert( NULL != dest_rect );
    assert ( NULL != out_solutions );
    assert ( NULL != out_solutions_count );
    assert ( 4 <= solutions_max );  /* current implementation requires at least 4 options */


    double good_dist = pencil_size_get_preferred_object_distance( &((*this_).pencil_size) );


    *out_solutions_count = 0;

    TRACE_END();
}

void pencil_layouter_private_connect_rectangles_by_L7 ( pencil_layouter_t *this_,
                                                        const geometry_rectangle_t *source_rect,
                                                        const geometry_rectangle_t *dest_rect,
                                                        uint32_t solutions_max,
                                                        geometry_connector_t out_solutions[],
                                                        uint32_t *out_solutions_count )
{
    TRACE_BEGIN();
    assert( NULL != source_rect );
    assert( NULL != dest_rect );
    assert ( NULL != out_solutions );
    assert ( NULL != out_solutions_count );
    assert ( 2 <= solutions_max );  /* current implementation requires at least 2 options */

    uint32_t solutions_count = 0;

    double src_left = geometry_rectangle_get_left(source_rect);
    double src_x_center = geometry_rectangle_get_x_center(source_rect);
    double src_right = geometry_rectangle_get_right(source_rect);
    double src_top = geometry_rectangle_get_top(source_rect);
    double src_y_center = geometry_rectangle_get_y_center(source_rect);
    double src_bottom = geometry_rectangle_get_bottom(source_rect);

    double dst_left = geometry_rectangle_get_left(dest_rect);
    double dst_x_center = geometry_rectangle_get_x_center(dest_rect);
    double dst_right = geometry_rectangle_get_right(dest_rect);
    double dst_top = geometry_rectangle_get_top(dest_rect);
    double dst_y_center = geometry_rectangle_get_y_center(dest_rect);
    double dst_bottom = geometry_rectangle_get_bottom(dest_rect);

    /* from source to left */
    if ( dst_x_center < src_left )
    {
        if ( dst_bottom < src_y_center )
        {
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_left,
                                                   src_y_center,
                                                   dst_x_center,
                                                   dst_bottom,
                                                   src_y_center
                                                 );
            solutions_count ++;
        }
        else if ( dst_top > src_y_center )
        {
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_left,
                                                   src_y_center,
                                                   dst_x_center,
                                                   dst_top,
                                                   src_y_center
                                                 );
            solutions_count ++;
        }
    }

    /* from source to right */
    if ( dst_x_center > src_right )
    {
        if ( dst_bottom < src_y_center )
        {
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_right,
                                                   src_y_center,
                                                   dst_x_center,
                                                   dst_bottom,
                                                   src_y_center
            );
            solutions_count ++;
        }
        else if ( dst_top > src_y_center )
        {
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_right,
                                                   src_y_center,
                                                   dst_x_center,
                                                   dst_top,
                                                   src_y_center
            );
            solutions_count ++;
        }
    }

    /* from source to upwards */
    if ( dst_y_center < src_top )
    {
        if ( dst_right < src_x_center )
        {
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_x_center,
                                                   src_top,
                                                   dst_right,
                                                   dst_y_center,
                                                   dst_y_center
            );
            solutions_count ++;
        }
        else if ( dst_left > src_x_center )
        {
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_x_center,
                                                   src_top,
                                                   dst_left,
                                                   dst_y_center,
                                                   dst_y_center
            );
            solutions_count ++;
        }
    }

    /* from source to downwards */
    if ( dst_y_center > src_bottom )
    {
        if ( dst_right < src_x_center )
        {
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_x_center,
                                                   src_bottom,
                                                   dst_right,
                                                   dst_y_center,
                                                   dst_y_center
            );
            solutions_count ++;
        }
        else if ( dst_left > src_x_center )
        {
            geometry_connector_reinit_horizontal ( &(out_solutions[solutions_count]),
                                                   src_x_center,
                                                   src_bottom,
                                                   dst_left,
                                                   dst_y_center,
                                                   dst_y_center
            );
            solutions_count ++;
        }
    }

    *out_solutions_count = solutions_count;

    TRACE_END();
}

data_id_t pencil_layouter_get_object_id_at_pos ( pencil_layouter_t *this_,
                                                 double x,
                                                 double y,
                                                 bool dereference )
{
    TRACE_BEGIN();

    data_id_t result;
    data_id_init_void( &result );
    data_diagram_t *diag;
    diag = pencil_input_data_get_diagram_ptr( (*this_).input_data );

    if ( geometry_rectangle_contains( &((*this_).diagram_bounds), x, y ) && data_diagram_is_valid(diag) )
    {
        /* check the relationship shapes */
        {
            uint32_t count_relations;
            count_relations = pencil_input_data_get_relationship_count ( (*this_).input_data );

            for ( uint32_t rel_index = 0; rel_index < count_relations; rel_index ++ )
            {
                geometry_connector_t *relationship_shape;
                relationship_shape = pencil_input_data_layout_get_relationship_shape_ptr( &((*this_).layout_data), rel_index );

                if ( geometry_connector_is_close( relationship_shape, x, y, 3.0 /*=distance*/ ) )
                {
                    data_relationship_t *current_relation;
                    current_relation = pencil_input_data_get_relationship_ptr ( (*this_).input_data, rel_index );

                    data_id_reinit( &result, DATA_TABLE_RELATIONSHIP, data_relationship_get_id( current_relation ) );
                }
            }
        }

        /* determine a classifier at the given position */
        if ( ! data_id_is_valid( &result ) )
        {
            result = pencil_layouter_private_get_classifier_id_at_pos( this_, x, y, dereference );
        }

        /* fallback: return the diagram */
        if ( ! data_id_is_valid( &result ) )
        {
            data_id_reinit( &result, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag) );
        }
    }
    else
    {
        TRACE_INFO( "no object at given location or no diagram chosen" );
        data_id_init_void( &result );
    }

    TRACE_END();
    return result;
}

data_id_t pencil_layouter_private_get_classifier_id_at_pos ( pencil_layouter_t *this_,
                                                             double x,
                                                             double y,
                                                             bool dereference )
{
    TRACE_BEGIN();
    data_id_t result;
    data_id_init_void( &result );

    if ( geometry_rectangle_contains( &((*this_).diagram_draw_area), x, y ) )
    {
        /* iterate over all classifiers */
        uint32_t count;
        count = pencil_input_data_get_visible_classifier_count ( (*this_).input_data );
        for ( uint32_t index = 0; index < count; index ++ )
        {
            data_visible_classifier_t *visible_classifier;
            visible_classifier = pencil_input_data_get_visible_classifier_ptr ( (*this_).input_data, index );

            if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
            {
                data_classifier_t *classifier;
                data_diagramelement_t *diagramelement;
                classifier = data_visible_classifier_get_classifier_ptr( visible_classifier );
                diagramelement = data_visible_classifier_get_diagramelement_ptr( visible_classifier );

                geometry_rectangle_t *classifier_bounds;
                geometry_rectangle_t *classifier_space;
                classifier_bounds = pencil_input_data_layout_get_classifier_bounds_ptr ( &((*this_).layout_data), index );
                classifier_space = pencil_input_data_layout_get_classifier_space_ptr ( &((*this_).layout_data), index );

                if ( geometry_rectangle_contains( classifier_bounds, x, y ) )
                {
                    if ( geometry_rectangle_contains( classifier_space, x, y ) )
                    {
                        /* check all contained features */
                        uint32_t linenumber = 0;
                        double lineheight = pencil_size_get_standard_font_size( &((*this_).pencil_size) )
                        + 2.0 * pencil_size_get_standard_object_border( &((*this_).pencil_size) );
                        for ( uint32_t f_idx = 0; f_idx < pencil_input_data_get_feature_count ( (*this_).input_data ); f_idx ++ )
                        {
                            data_feature_t *the_feature;
                            the_feature = pencil_input_data_get_feature_ptr ( (*this_).input_data, f_idx );
                            if ( data_feature_get_classifier_id( the_feature ) == data_classifier_get_id( classifier ) )
                            {
                                geometry_rectangle_t feature_bounds;
                                feature_bounds = pencil_layouter_get_feature_bounds( this_,
                                                                                     data_classifier_get_id( classifier ),
                                                                                     index,
                                                                                     f_idx,
                                                                                     linenumber
                                                                                   );
                                if ( geometry_rectangle_contains( &feature_bounds, x, y ) )
                                {
                                    data_id_reinit( &result, DATA_TABLE_FEATURE, data_feature_get_id( the_feature ) );
                                }
                                linenumber ++;
                                geometry_rectangle_destroy( &feature_bounds );
                            }
                        }
                    }
                    if ( ! data_id_is_valid( &result ) )
                    {
                        if ( dereference )
                        {
                            data_id_reinit( &result, DATA_TABLE_CLASSIFIER, data_classifier_get_id( classifier ) );
                        }
                        else
                        {
                            data_id_reinit( &result, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id( diagramelement ) );
                        }
                    }
                }
            }
            else
            {
                TSLOG_ERROR("invalid visible classifier in array!");
            }
        }
    }

    TRACE_END();
    return result;
}


/*
Copyright 2017-2017 Andreas Warnke

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
