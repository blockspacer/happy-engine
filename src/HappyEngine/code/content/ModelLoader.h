//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
//Author:	Bastian Damman
//Created:	23/08/2011
//Extended:	Sebastiaan Sprengers
//Removed concurrency queue because not cross platform: Bastian Damman - 29/10/2011
//Removed shared_ptr

#ifndef _HE_MODEL_LOADER_H_
#define _HE_MODEL_LOADER_H_
#pragma once

#include "Model.h"
#include "BufferLayout.h"
#include "AssetContainer.h"

namespace he {
namespace gfx {
    class Model;
    class ModelMesh;
};

namespace ct {

namespace models {
    class IModelLoader;
}

class ModelLoader
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    ModelLoader();
    virtual ~ModelLoader();
    
    /* GENERAL */
    void tick(float dTime); //checks for new load operations, if true start thread
    void glThreadInvoke();  //needed for all of the gl operations

    gfx::Model* asyncLoadModel(const std::string& path, const gfx::BufferLayout& vertexLayout);
    gfx::ModelMesh* asyncLoadModelMesh(const std::string& path, const std::string& meshName, const gfx::BufferLayout& vertexLayout);

    gfx::Model* loadModel(const std::string& path, const gfx::BufferLayout& vertexLayout);
    gfx::ModelMesh* loadModelMesh(const std::string& path, const std::string& meshName, const gfx::BufferLayout& vertexLayout);

    /* GETTERS */
    bool isLoading() const;

private:
    struct ModelLoadData
    {
    public:
        std::string path;
        gfx::BufferLayout vertexLayout;
        ObjectHandle modelHandle;
        models::IModelLoader* loader;
    };

    bool getModelLoader(ModelLoadData& data);
    bool startAsyncLoadModel(ModelLoadData& data);
    bool startSyncLoadModel(ModelLoadData& data);

    bool loadModel(ModelLoadData& data);
    bool createModel(ModelLoadData& data);

    bool isModelLoaded(const std::string& path, ObjectHandle& outHandle);
    void modelLoadThread();
    bool m_isModelThreadRunning;

    std::queue<ModelLoadData> m_ModelLoadQueue;
    boost::mutex m_ModelLoadQueueMutex;
    std::queue<ModelLoadData> m_ModelInvokeQueue;
    boost::mutex m_ModelInvokeQueueMutex;

    boost::mutex m_WaitListMutex;

    boost::thread m_ModelLoadThread;


    AssetContainer<ObjectHandle> m_AssetContainer;

    gfx::ModelMesh* m_EmptyMesh;
    

    //Disable default copy constructor and default assignment operator
    ModelLoader(const ModelLoader&);
    ModelLoader& operator=(const ModelLoader&);
};

} } //end namespace

#endif
