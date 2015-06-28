/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2010 Wayne Stambaugh <stambaughw@verizon.net>
 * Copyright (C) 2015 KiCad Developers, see CHANGELOG.TXT for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include <macros.h>
#include <trigo.h>
#include <transform.h>
#include <common.h>
#include <class_eda_rect.h>


TRANSFORM& TRANSFORM::operator=( const TRANSFORM& aTransform )
{
    if( this == &aTransform )       // Check for self assingnemt;
        return *this;

    x1 = aTransform.x1;
    y1 = aTransform.y1;
    x2 = aTransform.x2;
    y2 = aTransform.y2;
    return *this;
}


bool TRANSFORM::operator==( const TRANSFORM& aTransform ) const
{
    return ( x1 == aTransform.x1 &&
             y1 == aTransform.y1 &&
             x2 == aTransform.x2 &&
             y2 == aTransform.y2 );
}


wxPoint TRANSFORM::TransformCoordinate( const wxPoint& aPoint ) const
{
    return wxPoint( ( x1 * aPoint.x ) + ( y1 * aPoint.y ),
                    ( x2 * aPoint.x ) + ( y2 * aPoint.y ) );
}

EDA_RECT TRANSFORM::TransformCoordinate( const EDA_RECT& aRect ) const
{
    EDA_RECT rect;
    rect.SetOrigin( TransformCoordinate( aRect.GetOrigin() ) );
    rect.SetEnd( TransformCoordinate( aRect.GetEnd() ) );
    return rect;
}

/*
* Calculate the Inverse mirror/rotation transform.
*/
TRANSFORM TRANSFORM::InverseTransform( ) const
{
    int invx1;
    int invx2;
    int invy1;
    int invy2;

    /* Calculates the inverse matrix coeffs:
    * for a matrix m{x1, x2, y1, y2}
    * the inverse matrix is 1/(x1*y2 -x2*y1) m{y2,-x2,-y1,x1)
    */
    int det = x1*y2 -x2*y1; // Is never null, because the inverse matrix exists
    invx1 = y2/det;
    invx2 = -x2/det;
    invy1 = -y1/det;
    invy2 = x1/det;

    TRANSFORM invtransform( invx1, invy1, invx2, invy2 );
    return invtransform;
}


bool TRANSFORM::MapAngles( int* aAngle1, int* aAngle2 ) const
{
    wxCHECK_MSG( aAngle1 != NULL && aAngle2 != NULL, false,
                 wxT( "Cannot map NULL point angles." ) );

    int    Angle, Delta;
    double x, y, t;
    bool   swap = false;

    Delta = *aAngle2 - *aAngle1;
    if( Delta >= 1800 )
    {
        *aAngle1 -= 1;
        *aAngle2 += 1;
    }

    x = cos( DECIDEG2RAD( *aAngle1 ) );
    y = sin( DECIDEG2RAD( *aAngle1 ) );
    t = x * x1 + y * y1;
    y = x * x2 + y * y2;
    x = t;
    *aAngle1 = KiROUND( RAD2DECIDEG( atan2( y, x ) ) );

    x = cos( DECIDEG2RAD( *aAngle2 ) );
    y = sin( DECIDEG2RAD( *aAngle2 ) );
    t = x * x1 + y * y1;
    y = x * x2 + y * y2;
    x = t;
    *aAngle2 = KiROUND( RAD2DECIDEG( atan2( y, x ) ) );

    NORMALIZE_ANGLE_POS( *aAngle1 );
    NORMALIZE_ANGLE_POS( *aAngle2 );
    if( *aAngle2 < *aAngle1 )
        *aAngle2 += 3600;

    if( *aAngle2 - *aAngle1 > 1800 ) // Need to swap the two angles
    {
        Angle   = (*aAngle1);
        *aAngle1 = (*aAngle2);
        *aAngle2 = Angle;

        NORMALIZE_ANGLE_POS( *aAngle1 );
        NORMALIZE_ANGLE_POS( *aAngle2 );
        if( *aAngle2 < *aAngle1 )
            *aAngle2 += 3600;
        swap = true;
    }

    if( Delta >= 1800 )
    {
        *aAngle1 += 1;
        *aAngle2 -= 1;
    }

    return swap;
}


void TRANSFORM::ReOrient( enum ORIENTATION_T aOrientation )
{
    TRANSFORM temp = TRANSFORM();
    bool transform = false;

    // int cast suppresses warnings for combined flags
    switch( (int) aOrientation )
    {
    case ORIENT_0:
    case ORIENT_NORMAL:                    // default transform matrix
        x1 = 1;
        y2 = -1;
        x2 = y1 = 0;
        break;

    case ORIENT_ROTATE_COUNTERCLOCKWISE:  // Rotate + (incremental rotation)
        temp.x1   = temp.y2 = 0;
        temp.y1   = 1;
        temp.x2   = -1;
        transform = true;
        break;

    case ORIENT_ROTATE_CLOCKWISE:          // Rotate - (incremental rotation)
        temp.x1   = temp.y2 = 0;
        temp.y1   = -1;
        temp.x2   = 1;
        transform = true;
        break;

    case ORIENT_MIRROR_Y:                  // Mirror Y (incremental rotation)
        temp.x1   = -1;
        temp.y2   = 1;
        temp.y1   = temp.x2 = 0;
        transform = true;
        break;

    case ORIENT_MIRROR_X:                  // Mirror X (incremental rotation)
        temp.x1   = 1;
        temp.y2   = -1;
        temp.y1   = temp.x2 = 0;
        transform = true;
        break;

    case ORIENT_90:
        ReOrient( ORIENT_0 );
        ReOrient( ORIENT_ROTATE_COUNTERCLOCKWISE );
        break;

    case ORIENT_180:
        ReOrient( ORIENT_0 );
        ReOrient( ORIENT_ROTATE_COUNTERCLOCKWISE );
        ReOrient( ORIENT_ROTATE_COUNTERCLOCKWISE );
        break;

    case ORIENT_270:
        ReOrient( ORIENT_0 );
        ReOrient( ORIENT_ROTATE_CLOCKWISE );
        break;

    case ( ORIENT_0 + ORIENT_MIRROR_X ):
        ReOrient( ORIENT_0 );
        ReOrient( ORIENT_MIRROR_X );
        break;

    case ( ORIENT_0 + ORIENT_MIRROR_Y ):
        ReOrient( ORIENT_0 );
        ReOrient( ORIENT_MIRROR_Y );
        break;

    case ( ORIENT_90 + ORIENT_MIRROR_X ):
        ReOrient( ORIENT_90 );
        ReOrient( ORIENT_MIRROR_X );
        break;

    case ( ORIENT_90 + ORIENT_MIRROR_Y ):
        ReOrient( ORIENT_90 );
        ReOrient( ORIENT_MIRROR_Y );
        break;

    case ( ORIENT_180 + ORIENT_MIRROR_X ):
        ReOrient( ORIENT_180 );
        ReOrient( ORIENT_MIRROR_X );
        break;

    case ( ORIENT_180 + ORIENT_MIRROR_Y ):
        ReOrient( ORIENT_180 );
        ReOrient( ORIENT_MIRROR_Y );
        break;

    case ( ORIENT_270 + ORIENT_MIRROR_X ):
        ReOrient( ORIENT_270 );
        ReOrient( ORIENT_MIRROR_X );
        break;

    case ( ORIENT_270 + ORIENT_MIRROR_Y ):
        ReOrient( ORIENT_270 );
        ReOrient( ORIENT_MIRROR_Y );
        break;

    default:
        transform = false;
        wxMessageBox( wxT( "ReOrient() error: invalid orientation" ) );
        break;
    }

    if( transform )
    {
        /* The new matrix transform is the old matrix transform modified by the
         *  requested transformation, which is the temp transform (rot,
         *  mirror ..) in order to have (in term of matrix transform):
         *     transform coord = new_m_transform * coord
         *  where transform coord is the coord modified by new_m_transform from
         *  the initial value coord.
         *  new_m_transform is computed (from old_m_transform and temp) to
         *  have:
         *     transform coord = old_m_transform * temp
         */
        TRANSFORM newTransform;

        newTransform.x1 = x1 * temp.x1 + x2 * temp.y1;
        newTransform.y1 = y1 * temp.x1 + y2 * temp.y1;
        newTransform.x2 = x1 * temp.x2 + x2 * temp.y2;
        newTransform.y2 = y1 * temp.x2 + y2 * temp.y2;
        *this = newTransform;
    }
}

enum ORIENTATION_T TRANSFORM::GetOrientation() const
{
    int type_rotate = ORIENT_0;
    TRANSFORM temp;

#define ROTATE_VALUES_COUNT 12

    // list of all possibilities, but only the first ROTATE_VALUES_COUNT are actually used
    static const int rotate_value[ROTATE_VALUES_COUNT] =
    {
        ORIENT_0,                       ORIENT_90,
        ORIENT_180,                     ORIENT_270,
        ORIENT_MIRROR_X + ORIENT_0,     ORIENT_MIRROR_X + ORIENT_90,
        ORIENT_MIRROR_X + ORIENT_180,   ORIENT_MIRROR_X + ORIENT_270,
        ORIENT_MIRROR_Y + ORIENT_0,     ORIENT_MIRROR_Y + ORIENT_90,
        ORIENT_MIRROR_Y + ORIENT_180,   ORIENT_MIRROR_Y + ORIENT_270
    };

    temp = *this;

    for( int ii = 0; ii < ROTATE_VALUES_COUNT; ++ii )
    {
        type_rotate = rotate_value[ii];
        temp.ReOrient( (enum ORIENTATION_T) type_rotate );
        if( temp == *this )
            return (enum ORIENTATION_T) type_rotate;
    }

    // Error: orientation not found in list (should not happen)
    wxFAIL_MSG( _( "Component orientation matrix internal error" ) );
    return ORIENT_NORMAL;
};
