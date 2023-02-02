//Используем первый слот
cbuffer MVPConstantBuffer : register(b0)
{
    float4x4 model;
    float4x4 view;
    float4x4 projection;
};

struct VSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    // float2 outTexCoord : TEXCOORD;
};

VSInput main(float4 position : POSITION, float4 color : COLOR)
{
    VSInput result;
    float4 pos = position;
    pos = mul(pos, model);
    pos = mul(pos, view);
    pos = mul(pos, projection);
    result.position = pos;
    result.color = color;

    return result;
}




#define DECIMATION_RATIO 0.5  // Decrease triangle count by 50%
//
// cbuffer Constants
// {
//     float4x4 worldViewProj;
//     float decimationRatio;
// }
//
// struct VertexInput
// {
//     float3 position : POSITION;
//     float3 normal : NORMAL;
//     float2 texCoord : TEXCOORD;
// };
//
// struct VertexOutput
// {
//     float4 position : SV_POSITION;
//     float3 normal : NORMAL;
//     float2 texCoord : TEXCOORD;
// };

// VSInput main(VSInput input/*float4 position : POSITION, float4 color : COLOR*/)
// {
//          VSInput result;
//     VSInput output;
//     float4 pos = input.position;
//      pos = mul(pos, model);
//      pos = mul(pos, view);
//      pos = mul(pos, projection);
//      result.position = pos;
//     result.outTexCoord = input.outTexCoord;
//     // result.color = input.color;
//     output = result;
//
//      
//     // VertexOutput output;
//
//     // Perform vertex transformation
//     // output.position = mul(float4(result.position), worldViewProj);
//     // output.normal = input.normal;
//     // output.texCoord = input.texCoord;
//
//     // Randomly discard vertices based on decimation ratio
//     // if (0.8 < frac(result.position.x * result.position.y * result.position.z * 31337.0)){}
//     // output.position.w = 0.0;
//     return output;
//     // return output;
// }






















// // Declare the vertex-to-pixel projection matrix.
// float4x4 projectionMatrix;
//
// // Declare the threshold distance parameter.
// float thresholdDistance;
//
// // Define the main function of the vertex shader.
// VS_OUTPUT main(VS_INPUT input)
// {
//     // Initialize the output position to the input position.
//     VS_OUTPUT output;
//     output.position = float4(input.position, 1.0);
//
//     // Transform the position from object space to clip space.
//     output.position = mul(output.position, projectionMatrix);
//
//     // Discard the vertex if it is below the threshold distance.
//     if (output.position.z < thresholdDistance)
//         discard;
//
//     return output;
// }
// // Declare the vertex-to-pixel projection matrix.
// float4x4 projectionMatrix;
//
// // Set up the projection matrix using the D3DXMatrixPerspectiveFovLH function.
// projectionMatrix = D3DXMatrixPerspectiveFovLH(
//     fov,        // Field of view (in radians)
//     aspectRatio, // Aspect ratio (width / height)
//     nearClip,   // Near clip plane distance
//     farClip);







//
//
// struct VertexInput
// {
//     float3 position : POSITION;
// };
//
// struct VertexOutput
// {
//     float4 position : SV_POSITION;
// };
//
// cbuffer HalfEdgeCollapseParams
// {
//     int numHalfEdgesToCollapse;
//     int2 halfEdgeIndices[MAX_HALF_EDGES]; // Indices of the half-edges to collapse
//     float3 collapsePositions[MAX_HALF_EDGES]; // New positions of the vertices after collapse
// }
//
// VertexOutput main(VertexInput input)
// {
//     VertexOutput output;
//
//     // Initialize the output position to the input position
//     output.position = float4(input.position, 1.0);
//
//     // Iterate over the list of half-edges to collapse
//     for (int i = 0; i < numHalfEdgesToCollapse; i++)
//     {
//         // If the current vertex is one of the vertices connected by the half-edge, update its position
//         if (halfEdgeIndices[i].x == gl_VertexID || halfEdgeIndices[i].y == gl_VertexID)
//         {
//             output.position = float4(collapsePositions[i], 1.0);
//             break;
//         }
//     }
//
//     return output;
// }