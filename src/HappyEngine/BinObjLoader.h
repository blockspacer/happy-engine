//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyEngine.
//
//    HappyEngine is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyEngine is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.
//
//Author: Bastian Damman
//Created: 28/08/2011

#ifndef _HE_BINOBJ_LOADER_H_
#define _HE_BINOBJ_LOADER_H_
#pragma once

#include <string>
#include <vector>
#include <map>

#include "VertexLayout.h"
#include "boost/shared_ptr.hpp"
#include "Vector3.h"
#include "Vector2.h"
#include "Model.h"
#include "HappyTypes.h"
#include "IModelLoader.h"

namespace happyengine {
namespace content {
namespace models {

class BinObjLoader : public IModelLoader
{
public:
   struct InternalVertex
   {
       math::Vector3 pos;
       math::Vector2 tex;
       math::Vector3 norm;
       math::Vector3 tan;

       InternalVertex(math::Vector3 p, math::Vector2 t, math::Vector3 n, math::Vector3 tn):
                            pos(p), tex(t), norm(n), tan(tn)
       {}    
       InternalVertex(): pos(), tex(), norm(), tan()
       {} 
   };
	BinObjLoader();
    virtual ~BinObjLoader();

    virtual void load(const std::string& path, const graphics::VertexLayout& vertLayout, bool allowByteIndices = true);

    virtual uint getNumMeshes() const;
    virtual const std::string& getMeshName(uint mesh) const;

    virtual const void* getVertices(uint mesh) const;
    virtual uint getNumVertices(uint mesh) const;

    virtual const void* getIndices(uint mesh) const;
    virtual graphics::IndexStride getIndexStride(uint mesh) const;
    virtual uint getNumIndices(uint mesh) const;


private:
    void read(const std::string& path, bool allowByteIndices);
    void fill(const graphics::VertexLayout& vertLayout) const;

    std::vector<std::vector<InternalVertex>> m_VertexData;
    
    std::vector<std::string> m_MeshName;

    std::vector<void*> m_Vertices;
    std::vector<void*> m_Indices;

    std::vector<uint> m_NumIndices;
    std::vector<graphics::IndexStride> m_IndexStride;

    //Disable default copy constructor and default assignment operator
    BinObjLoader(const BinObjLoader&);
    BinObjLoader& operator=(const BinObjLoader&);
};

} } } //end namespace

#endif
