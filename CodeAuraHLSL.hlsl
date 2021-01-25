#if SCENE_TEXTURES_DISABLED
return 0;
#endif

float LinearStepSize = OutlineSize / NbLinearSteps;
float RadialStepSize = 2*3.14159265f / NbRadialSteps;

float result, stencil = 0.f;
for( int i = 1; i <= NbLinearSteps; i++){
    float currLinearStep = i * LinearStepSize;
    for (int j = 0; j< NbRadialSteps; ++j){
        float angle = j * RadialStepSize;
        float2 coord = ViewportUVToBufferUV(GetViewportUV(Parameters) + currLinearStep*float2(cos(angle),sin(angle)));

        FScreenSpaceData ScreenSpaceData = GetScreenSpaceData(coord, false);

        bool bIsNotOccluded = (ScreenSpaceData.GBuffer.CustomDepth.r - ScreenSpaceData.GBuffer.Depth.r) == 0;
        if(ScreenSpaceData.GBuffer.CustomStencil.r != 0 && bIsNotOccluded)
        {
            result++;
            stencil = max(stencil, ScreenSpaceData.GBuffer.CustomStencil.r);
        }
    }
}

result /= NbRadialSteps * NbLinearSteps;

return float2(result,stencil);