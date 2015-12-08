/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2015 Cirilo Bernardo <cirilo.bernardo@gmail.com>
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

#include <iostream>
#include <3d_cache/sg/ifsg_coords.h>
#include <3d_cache/sg/sg_coords.h>


extern char BadObject[];
extern char BadParent[];
extern char WrongParent[];


IFSG_COORDS::IFSG_COORDS( bool create )
{
    m_node = NULL;

    if( !create )
        return ;

    m_node = new SGCOORDS( NULL );

    if( m_node )
        m_node->AssociateWrapper( &m_node );

    return;
}


IFSG_COORDS::IFSG_COORDS( SGNODE* aParent )
{
    m_node = new SGCOORDS( NULL );

    if( m_node )
    {
        if( !m_node->SetParent( aParent ) )
        {
            delete m_node;
            m_node = NULL;
            std::cerr << __FILE__ << ": " << __FUNCTION__ << ": " << __LINE__ << "\n";
            std::cerr << WrongParent << "\n";
            return;
        }

        m_node->AssociateWrapper( &m_node );
    }

    return;
}


IFSG_COORDS::IFSG_COORDS( IFSG_NODE& aParent )
{
    SGNODE* pp = aParent.GetRawPtr();

    if( ! pp )
    {
        std::cerr << __FILE__ << ": " << __FUNCTION__ << ": " << __LINE__ << "\n";
        std::cerr << BadParent << "\n";
    }

    m_node = new SGCOORDS( NULL );

    if( m_node )
    {
        if( !m_node->SetParent( pp ) )
        {
            delete m_node;
            m_node = NULL;
            std::cerr << __FILE__ << ": " << __FUNCTION__ << ": " << __LINE__ << "\n";
            std::cerr << WrongParent << "\n";
            return;
        }

        m_node->AssociateWrapper( &m_node );
    }

    return;
}


bool IFSG_COORDS::Attach( SGNODE* aNode )
{
    if( m_node )
        m_node->DisassociateWrapper( &m_node );

    m_node = NULL;

    if( !aNode )
        return false;

    if( S3D::SGTYPE_COORDS != aNode->GetNodeType() )
    {
        return false;
    }

    m_node = aNode;
    m_node->AssociateWrapper( &m_node );

    return true;
}


bool IFSG_COORDS::NewNode( SGNODE* aParent )
{
    if( m_node )
        m_node->DisassociateWrapper( &m_node );

    m_node = new SGCOORDS( aParent );

    if( aParent != m_node->GetParent() )
    {
        std::cerr << __FILE__ << ": " << __FUNCTION__ << ": " << __LINE__ << "\n";
        std::cerr << " * [BUG] invalid SGNODE parent (";
        std::cerr << aParent->GetNodeTypeName( aParent->GetNodeType() );
        std::cerr << ") to SGCOORDS\n";
        delete m_node;
        m_node = NULL;
        return false;
    }

    m_node->AssociateWrapper( &m_node );

    return true;
}


bool IFSG_COORDS::NewNode( IFSG_NODE& aParent )
{
    SGNODE* np = aParent.GetRawPtr();

    if( NULL == np )
    {
        std::cerr << __FILE__ << ": " << __FUNCTION__ << ": " << __LINE__ << "\n";
        std::cerr << BadParent << "\n";
        return false;
    }

    return NewNode( np );
}


bool IFSG_COORDS::GetCoordsList( size_t& aListSize, SGPOINT*& aCoordsList )
{
    if( NULL == m_node )
    {
        std::cerr << __FILE__ << ": " << __FUNCTION__ << ": " << __LINE__ << "\n";
        std::cerr << BadObject << "\n";
        return false;
    }

    return ((SGCOORDS*)m_node)->GetCoordsList( aListSize, aCoordsList );
}


bool IFSG_COORDS::SetCoordsList( size_t aListSize, const SGPOINT* aCoordsList )
{
    if( NULL == m_node )
    {
        std::cerr << __FILE__ << ": " << __FUNCTION__ << ": " << __LINE__ << "\n";
        std::cerr << BadObject << "\n";
        return false;
    }

    ((SGCOORDS*)m_node)->SetCoordsList( aListSize, aCoordsList );

    return true;
}


bool IFSG_COORDS::AddCoord( double aXValue, double aYValue, double aZValue )
{
    if( NULL == m_node )
    {
        std::cerr << __FILE__ << ": " << __FUNCTION__ << ": " << __LINE__ << "\n";
        std::cerr << BadObject << "\n";
        return false;
    }

    ((SGCOORDS*)m_node)->AddCoord( aXValue, aYValue, aZValue );

    return true;
}


bool IFSG_COORDS::AddCoord( const SGPOINT& aPoint )
{
    if( NULL == m_node )
    {
        std::cerr << __FILE__ << ": " << __FUNCTION__ << ": " << __LINE__ << "\n";
        std::cerr << BadObject << "\n";
        return false;
    }

    ((SGCOORDS*)m_node)->AddCoord( aPoint );

    return true;
}
