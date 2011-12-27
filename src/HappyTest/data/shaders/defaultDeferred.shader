[Shader]
vsPath = "deferred/pre/deferredPreShader.vert"
fsPath = "deferred/pre/deferredPreShader.frag"

[info]
translucent = false

[in]
inPosition = POSITION
inTexCoord = TEXCOORD
inNormal = NORMAL
inTangent = TANGENT

[uniform]
matWVP = WORLDVIEWPROJECTION
matWorld = WORLDVIEW
diffuseMap = TEXTURE2D
normalMap = TEXTURE2D
specGlossIllMap = TEXTURE2D

[out]
outColor = GBUFFER_COLOR
outSGI = GBUFFER_SGI
outNormal = GBUFFER_NORMAL
