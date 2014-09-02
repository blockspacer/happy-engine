{
    "Normal" : {
        "Normal" : {
            "VertexShader" : "deferred/pre/deferredPreShader.vert",
            "FragmentShader" : "deferred/pre/deferredPreShader.frag",
            "OutputLayout" : [ "outColor", "outSG", "outNormalDepth" ]
        },
        "Skinned" : {
            "VertexShader" : "deferred/pre/deferredPreShaderSkinned.vert",
            "FragmentShader" : "deferred/pre/preShadowShader.frag",
            "OutputLayout" : [ "outColor", "outSG", "outNormalDepth" ]
        },
        "Instanced" : {
            "VertexShader" : "deferred/pre/deferredPreShaderInstanced.vert",
            "FragmentShader" : "deferred/pre/preShadowShader.frag",
            "OutputLayout" : [ "outColor", "outSG", "outNormalDepth" ]
        }
    },
    "Shadow" : {
        "Normal" : {
            "VertexShader" : "shadow/preShadowShader.vert",
            "FragmentShader" : "shadow/preShadowShader.frag"
        },
        "Skinned" : {
            "VertexShader" : "shadow/preShadowShaderSkinned.vert",
            "FragmentShader" : "shadow/preShadowShader.frag"
        },
        "Instanced" : {
            "VertexShader" : "shadow/preShadowShaderInstanced.vert",
            "FragmentShader" : "shadow/preShadowShader.frag"
        }
    },
    "Options" : {
        "IsBlended" : false,
        "NoPost" : false,
        "CullFrontFace" : false,
        "DepthRead" : true,
        "DepthWrite" : true,
        "CastShadow" : true,
        "Parameters" : {
            "diffuseMap" : "0.5, 0.5, 0.5, 1.0",
            "normalMap" : "0.5, 0.5, 1.0, 1.0",
            "specGlossIllMap" : "0.2, 0.2, 0.0, 1.0"
        },
    }
}