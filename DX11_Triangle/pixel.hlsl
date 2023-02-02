struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

float4 main(PSInput input) : SV_TARGET
{
    
	return input.color;
}


float4x4 Inv(float4x4 mat)
{
	float4x4 inverse;
	float m00 = mat[0][0], m01 = mat[0][1], m02 = mat[0][2], m03 = mat[0][3],
	      m10 = mat[1][0], m11 = mat[1][1], m12 = mat[1][2], m13 = mat[1][3],
	      m20 = mat[2][0], m21 = mat[2][1], m22 = mat[2][2], m23 = mat[2][3],
	      m30 = mat[3][0], m31 = mat[3][1], m32 = mat[3][2], m33 = mat[1][3];
	// Calculate the determinant of the matrix
    float det = m00 * (m11 * m22 - m21 * m12) -
		m01 * (m10 * m22 - m12 * m20) +
		m02 * (m10 * m21 - m11 * m20);

	// Check if the matrix is invertible
	// if (abs(det) < 1e-6)
	// {
	// 	// Handle non-invertible matrix
	// 	// (e.g. return identity matrix or throw an exception)
 //        return nullptr;
 //    }

	// Calculate the inverse of the matrix
	float invdet = 1.0f / det;
	inverse[0][0] = (m11 * m22 - m21 * m12) * invdet;
	inverse[0][1] = (m02 * m21 - m01 * m22) * invdet;
	inverse[0][2] = (m01 * m12 - m02 * m11) * invdet;
	inverse[0][3] = 0.0f;
	inverse[1][0] = (m12 * m20 - m10 * m22) * invdet;
	inverse[1][1] = (m00 * m22 - m02 * m20) * invdet;
	inverse[1][2] = (m10 * m02 - m00 * m12) * invdet;
	inverse[1][3] = 0.0f;
	inverse[2][0] = (m10 * m21 - m20 * m11) * invdet;
	inverse[2][1] = (m20 * m01 - m00 * m21) * invdet;
	inverse[2][2] = (m00 * m11 - m10 * m01) * invdet;
	inverse[2][3] = 0.0f;
	inverse[3][0] = -(m10 * m00 + m31 * m10 + m32 * m20);
	inverse[3][1] = -(m30 * m01 + m31 * m11 + m32 * m21);
	inverse[3][2] = -(m30 * m02 + m31 * m12 + m32 * m22);
	inverse[3][3] = 1.0f;
	return inverse;
}

// float4 main(PSInput input) : SV_TARGET
// {
//     
// 	// return input.color;
//
// float4 vPos = float4(0, 0, 0,0);
//
//     //Read the vertex average from the cluster map;
//    
//
// int iCount = mDataWorld.w;
//     //Only compute optimal position if there are vertices in this cluster
//     if(iCount !=0)
//     {
// 	    //Read all the data from the clustermap to recunstruct the quadratic
//         mDataA0 = tClusterMap1.SampleLevel(sClusterMap1, vTexCoord.tex_coord, 0);
//         mDataA1 = tClusterMap2.SampleLevel(sClusterMap2, vTexCoord.tex_coord, 0);
//         mDataB = tClusterMap3.SampleLevel(sClusterMap3, vTexCoord.tex_coord, 0);
//
//         // re-assembling the quadric
//         float4x4 Q;
//
// float4 qB = float4(0,0,0,1);
// float qC = mDataA1.z;
//
//         //Determine if inverting A is stable, if so, compute optimal position
//         // If not, defaut to using the average position
// const float SINGULAR_THRESHOLD = 1e-11;
// 		// if (determinant( quadricA) > SINGULAR_THRESHOLD)
//   //           vPos = -mul(SymInv(quadricA), quadricB);
//         if (determinant(Q) > SINGULAR_THRESHOLD)
//             vPos = mul(Inv(Q), qB);
//         else
//             vPos = mDataWorld.xyz / mDataWorld.w;
//     }
//     input.position = vPos;
//     return input.color;
//
// }



/*
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

*/


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
