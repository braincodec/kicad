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

/**
 * @file ifsg_api.h
 * defines the API calls for the manipulation of SG* classes
 */

#ifndef IFSG_API_H
#define IFSG_API_H

#include <wx/string.h>
#include <glm/glm.hpp>
#include <3d_cache/sg/sg_types.h>
#include <3d_cache/sg/sg_base.h>
#include <3d_rendering/c3dmodel.h>

class SGNODE;
class SCENEGRAPH;
struct S3D_INFO;
struct S3D_POINT;

namespace S3D
{
    /**
     * Function WriteCache
     * writes the SGNODE tree to a binary cache file
     *
     * @param aFileName is the name of the file to write
     * @param overwrite must be set to true to overwrite an existing file
     * @param aNode is any node within the node tree which is to be written
     * @return true on success
     */
    bool WriteCache( const wxString& aFileName, bool overwrite, SGNODE* aNode );

    /**
     * Function ReadCache
     * reads a binary cache file and creates an SGNODE tree
     *
     * @param aFileName is the name of the binary cache file to be read
     * @return NULL on failure, on success a pointer to the top level SCENEGRAPH node;
     * if desired this node can be associated with an IFSG_TRANSFORM wrapper via
     * the IFSG_TRANSFORM::Attach() function.
     */
    SGNODE* ReadCache( const wxString& aFileName );

    /**
     * Function WriteVRML
     * writes out the given node and its subnodes to a VRML2 file
     *
     * @param filename is the name of the output file
     * @param overwrite should be set to true to overwrite an existing VRML file
     * @param aTopNode is a pointer to a SCENEGRAPH object representing the VRML scene
     * @param reuse should be set to true to make use of VRML DEF/USE features
     * @return true on success
     */
    bool WriteVRML( const wxString& filename, bool overwrite, SGNODE* aTopNode,
                    bool reuse, bool renameNodes );

    // NOTE: The following functions are used in combination to create a VRML
    // assembly which may use various instances of each SG* representation of a module.
    // A typical use case would be:
    // 1. invoke 'ResetNodeIndex()' to reset the global node name indices
    // 2. for each model pointer provided by 'S3DCACHE->Load()', invoke 'RenameNodes()' once;
    //    this ensures that all nodes have a unique name to present to the final output file.
    //    Internally, RenameNodes() will only rename the given node and all Child subnodes;
    //    nodes which are only referenced will not be renamed. Using the pointer supplied
    //    by 'S3DCACHE->Load()' ensures that all nodes but the returned node (top node) are
    //    children of at least one node, so all nodes are given unique names.
    // 3. if SG* trees are created independently of S3DCACHE->Load() the user must invoke
    //    RenameNodes() as appropriate to ensure that all nodes have a unique name
    // 4. create an assembly structure by creating new IFSG_TRANSFORM nodes as appropriate
    //    for each instance of a component; the component base model as returned by
    //    S3DCACHE->Load() may be added to these IFSG_TRANSFORM nodes via 'AddRefNode()';
    //    set the offset, rotation, etc of the IFSG_TRANSFORM node to ensure correct
    // 5. Ensure that all new IFSG_TRANSFORM nodes are placed as child nodes within a
    //    top level IFSG_TRANSFORM node in preparation for final node naming and output
    // 6. Invoke RenameNodes() on the top level assembly node
    // 7. Invoke WriteVRML() as normal, with renameNodes = false, to write the entire assembly
    //    structure to a single VRML file
    // 8. Clean up by deleting any extra IFSG_TRANSFORM wrappers and their underlying SG*
    //    classes which have been created solely for the assembly output

    /**
     * Function ResetNodeIndex
     * resets the global SG* class indices
     *
     * @param aNode may be any valid SGNODE
     */
    void ResetNodeIndex( SGNODE* aNode );

    /**
     * Function RenameNodes
     * renames a node and all children nodes based on the current
     * values of the global SG* class indices
     *
     * @param aNode is a top level node
     */
    void RenameNodes( SGNODE* aNode );

    /**
     * Function DestroyNode
     * deletes the given SG* class node. This function makes it possible
     * to safely delete an SG* node without associating the node with
     * its corresponding IFSG* wrapper.
     */
    void DestroyNode( SGNODE* aNode );

    // NOTE: The following functions facilitate the creation and destruction
    // of data structures for rendering

    /**
     * Function Prepare
     * creates an S3DMODEL representation of aNode transformed by aTransform.
     *
     * @param aNode is the node to be transcribed into an S3DMODEL representation
     * @param aTransform is the transform (Translation * Rotation * Scale) to apply
     * to the representation of aNode; it must be a pointer to a glm::dmat4 entity
     *
     * @return an S3DMODEL representation of aNode on success, otherwise NULL
     */
    S3DMODEL* Prepare( SCENEGRAPH* aNode, const glm::dmat4* aTransform );

    /**
     * Function Destroy3DModel
     * frees memory used by an S3DMODEL structure and sets the pointer to
     * the structure to NULL
     */
    void Destroy3DModel( S3DMODEL** aModel );

    /**
     * Function Free3DModel
     * frees memory used internally by an S3DMODEL structure
     */
    void Free3DModel( S3DMODEL& aModel );

    /**
     * Function Free3DMesh
     * frees memory used internally by an SMESH structure
     */
    void Free3DMesh( SMESH& aMesh );

    /**
     * Function New3DModel
     * creates and initializes an S3DMODEL struct
     */
    S3DMODEL* New3DModel( void );

    /**
     * Function Init3DMaterial
     * initializes an SMATERIAL struct
     */
    void Init3DMaterial( SMATERIAL& aMat );

    /**
     * Function Init3DMesh
     * creates and initializes an SMESH struct
     */
    void Init3DMesh( SMESH& aMesh );
};

#endif  // IFSG_API_H
