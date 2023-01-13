// struct PSInput
// {
//     float4 position : SV_POSITION;
//     float4 color : COLOR;
// };
// float4 main(PSInput input) : SV_TARGET
// {
//     return input.color;
// }

struct PSInput
{
    float4 position : SV_POSITION;
    float2 tex_coord : COLOR;
};

Texture2D tClusterMap0 : TEXTURE : register(t0);
Texture2D tClusterMap1 : TEXTURE : register(t1);
Texture2D tClusterMap2 : TEXTURE : register(t2);
Texture2D tClusterMap3 : TEXTURE : register(t3);

SamplerState sClusterMap0 : SAMPLER : register(s0);
SamplerState sClusterMap1 : SAMPLER : register(s1);
SamplerState sClusterMap2 : SAMPLER : register(s2);
SamplerState sClusterMap3 : SAMPLER : register(s3);

float3x3 SymInv( float3x3 A)
{
    float a11 = A[0][0], a12 = A[0][1], a13 = A[0][2], a22 = A[1][1],
	a23 = A[1][2], a33 = A[2][2];

    float inv11 = a22 * a33 - a23 * a23;
    float inv12 = -a12 * a33 + a13 * a23;
    float inv13 = a12 * a23 - a13 * a22;
    float inv22 = a11 * a33 - a13 * a13;
    float inv23 = -a11 * a23 + a12 * a13;
    float inv33 = a11 * a22 - a12 * a12;

    float detA = a11 * inv11 + a12 * inv12 + a13 * inv13;

    float3x3 Ainv =
    {
        inv11, inv12, inv13,
					inv12, inv22, inv23,
					inv13, inv23, inv33
    };
    return Ainv / detA;
}

//Pixel shader: input from the application is a full screen quad [0,1] texcoords

float3 main(PSInput vTexCoord) : SV_TARGET
{
    float3 vPos = float3(0, 0, 0);
    float4 mDataWorld, mDataA0, mDataB, mDataA1;
    //Read the vertex average from the cluster map;
    mDataWorld = tClusterMap0.SampleLevel(sClusterMap0, vTexCoord.tex_coord, 0);

    int iCount = mDataWorld.w;
    //Only compute optimal position if there are vertices in this cluster
    if(iCount !=0)
    {
	    //Read all the data from the clustermap to recunstruct the quadratic
        mDataA0 = tClusterMap1.SampleLevel(sClusterMap1, vTexCoord.tex_coord, 0);
        mDataA1 = tClusterMap2.SampleLevel(sClusterMap2, vTexCoord.tex_coord, 0);
        mDataB = tClusterMap3.SampleLevel(sClusterMap3, vTexCoord.tex_coord, 0);

        // re-assembling the quadric
        float3x3 qA =
        {
            mDataA0.x, mDataA0.y, mDataA0.z,
            mDataA0.y, mDataA0.w, mDataA1.x,
            mDataA0.z, mDataA1.x, mDataA1.y};

        float3 qB = mDataB.xyz;
        float qC = mDataA1.z;

        //Determine if inverting A is stable, if so, compute optimal position
        // If not, defaut to using the average position
        const float SINGULAR_THRESHOLD = 1e-11;
		// if (determinant( quadricA) > SINGULAR_THRESHOLD)
  //           vPos = -mul(SymInv(quadricA), quadricB);
        if (determinant(qA) > SINGULAR_THRESHOLD)
            vPos = -mul(SymInv(qA), qB);
        else
            vPos = mDataWorld.xyz / mDataWorld.w;
    }
    return vPos;



}

//Read the vertex average from the cluster map


/*
// Create a texture description for the cluster map
D3D11_TEXTURE2D_DESC clusterMapDesc;
clusterMapDesc.Width = width;
clusterMapDesc.Height = height;
clusterMapDesc.MipLevels = 1;
clusterMapDesc.ArraySize = 1;
clusterMapDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
clusterMapDesc.SampleDesc.Count = 1;
clusterMapDesc.SampleDesc.Quality = 0;
clusterMapDesc.Usage = D3D11_USAGE_DEFAULT;
clusterMapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
clusterMapDesc.CPUAccessFlags = 0;
clusterMapDesc.MiscFlags = 0;

// Create the cluster map texture
ID3D11Texture2D* clusterMapTexture;
hr = pDevice->CreateTexture2D(&clusterMapDesc, NULL, &clusterMapTexture);
if (FAILED(hr))
{
    // Error creating cluster map texture
    return hr;
}

// Create a shader resource view for the cluster map texture
D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
srvDesc.Format = clusterMapDesc.Format;
srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
srvDesc.Texture2D.MipLevels = 1;
srvDesc.Texture2D.MostDetailedMip = 0;

ID3D11ShaderResourceView* clusterMapSRV;
hr = pDevice->CreateShaderResourceView(clusterMapTexture, &srvDesc, &clusterMapSRV);
if (FAILED(hr))
{
    // Error creating shader resource view for cluster map texture
    return hr;
}

// You can then bind the cluster map SRV to a shader using the ID3D11DeviceContext::PSSetShaderResources method.
This code creates a 2D texture and a shader resource view for the cluster map, but you can also create a 3D texture if needed. The width and height of the texture should match the resolution of the mesh that the cluster map will be used for. The format of the texture is specified as DXGI_FORMAT_R32G32B32A32_FLOAT, which means it stores 4 32-bit floating point values per texel. The values in the cluster map can then be read and written to using a pixel shader.




*/