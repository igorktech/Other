#include "def.h"


struct Vertex
{
    float3 position;
    float4 color;
};
// Буфер позиций в слоте 0
RWStructuredBuffer<float2> position : register(u0);
// Буфер индексов, UAV в слоте 0
RWStructuredBuffer<uint> indexBuffer : register(u1);


// [numthreads(NUMTHREADS, 1, 1)]
// void main( uint3 DTid : SV_DispatchThreadID )
// {
// }

// #define NUM_THREADS 64
//
// cbuffer ConstantBuffer : register(b0)
// {
//     float4x4 WorldViewProjection;
//     float4 EyePosition;
//     float4 LightDirection;
//     float4 DiffuseColor;
//     float4 SpecularColor;
//     float4 AmbientColor;
//     float4 EmissiveColor;
//     float4 Reflectance;
//     float4 Transparency;
//     float4 Shininess;
// }
//
// struct Vertex
// {
//     float4 Position : POSITION;
//     float4 Normal : NORMAL;
//     float2 TexCoord : TEXCOORD;
// };
//
// StructuredBuffer<Vertex> vertexBuffer : register(t0);
//
// RWStructuredBuffer<uint> indexBuffer : register(u0);
//
// [numthreads(NUM_THREADS, 1, 1)]
// void CSMain(uint3 dispatchThreadId : SV_DispatchThreadID)
// {
//     // uint index = dispatchThreadId.x;
//     uint numElements = indexBuffer.GetDimensions(0).x;
//     if (index < numElements)
//     {
//         // Delete triangle by setting index to 0
//         indexBuffer[index] = 0;
//     }
// }
[numthreads(64, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
 //     // uint indexBuffer = id.x;
 //    uint index = id.x;
 //    //
 //    // if (index < indexBuffer.NumElements)
 //    // {
 //    //     // Delete triangle by setting index to 0
 //    //     indexBuffer[index] = 0;
 //    // }
 //    float3 acc = float3(1, 1,1);
 //    for (uint i = 0; i < index; i++)
 //    {
 //        // Вектор от одной точки до другой
 //        float3 diff = position[i].position - position[id.x].position;
 //        // Берем минимальное значение модуля вектора, чтобы не рассматривать случай 0-вектора
 //        float len = max(1e-10, length(diff));
 //        float k = 1e-9 * (len - 0.25) * (len - 0.25) / len;
 //        acc += k * diff;
 //    }
 //    // // Vertex tmp;
 //    // // tmp.color = position[id.x].color;
 //    // // tmp.position = position[id.x].position +
 //    // // 0.5 * acc;
 //    // // tmp.position = position[id.x].position.y +
 //    // // 0.5 * acc;
 //    // // tmp.position = position[id.x].position.z +
 //    // // 0.5 * acc;
 //    position[id.x].position = float3(0, 0, 0);
 // // = 0 * acc;
 //    // velocity[id.x] += acc;
 //    indexBuffer[id.x] = 0;
    //////uint index = id.x;
    //////float2 acc = float2(0, 0);
    //////for (uint i = 0; i < 20000; i++)
    //////{
    //////    // Вектор от одной точки до другой
    //////    float2 diff = position[i] - position[id.x];
    //////    // Берем минимальное значение модуля вектора, чтобы не рассматривать случай 0-вектора
    //////    float len = max(1e-10, length(diff));
    //////    float k = 1e-9 * (len - 0.25) / len;
    //////    acc += k * diff;
    //////}

    //////position[id.x] += /*velocity[id.x] +*/20000.5 * acc;
}


//
// In DirectX12,
// the Dynamic
// LOD sample
// demonstrates how
// to use
// the dynamic
// level ofdetail (LOD)
// feature to
// optimize the
// rendering of
// a scene
// by selecting
// the most
// appropriate LODfor
// each mesh
// based on
// the distance
// from thecamera.
// The sample
// uses an
// indirect draw
// command buffer
// and a
// compute shader
// to select
// the LODfor
// each mesh.
//
// To implement
// a similarfeature in
// DirectX11,
// you can
// use a
// similar approach
// by using
// an indirect
// draw command
// buffer and
// a compute
// shader to
// select theLOD for
// each mesh.
// Here is
// an example
// of how
// this can
// be done :
//
// Computeshader:
//
// Copy code
// #define NUM_THREADS 64
//
// // Declare the mesh data buffers as structured buffers
// StructuredBuffer<float3> positions : register(t0);
// StructuredBuffer<float3> normals : register(t1);
// StructuredBuffer<float2> texcoords : register(t2);
// StructuredBuffer<uint> indices : register(t3);
//
// // Declare the LOD data buffer as a structured buffer
// StructuredBuffer<uint> LODs : register(t4);
//
// // Declare the indirect draw command buffer as a structured buffer
// StructuredBuffer<DrawIndexedInstancedIndirectCommand> commands : register(u0);
//
// // Declare the constant buffers
// cbuffer TransformBuffer
// {
//     matrix model;
//     matrix view;
//     matrix projection;
// }
//
// cbuffer CameraBuffer
// {
//     float3 cameraPosition;
// }
//
// [numthreads(NUM_THREADS, 1, 1)]
// void CSMain(uint3 dispatchThreadId : SV_DispatchThreadID)
// {
//     uint index = dispatchThreadId.x;
//
//     if (index < commands.NumElements)
//     {
//         // Read the indirect draw command
//         DrawIndexedInstancedIndirectCommand command = commands[index];
//
//         // Calculate the distance from the camera to the mesh
//         float3 meshPosition = positions[command.StartIndex];
//         float3 cameraToMesh = meshPosition - cameraPosition;
//         float distance = length(cameraToMesh);
//
//         // Select the appropriate LOD based on the distance
//         // You will need to define the logic for this based on your specific needs
//         uint LOD = SelectLOD(distance);
//
//         // Update the LOD data buffer
//         LODs[index] = LOD;
//
//         // Update the indirect draw command with the new index count and instance count
//         // You will need to define the logic for this based on your specific needs
//         UpdateDrawCommand(command, LOD);
//
//         // Write the updated draw command back to the buffer
//         commands[index] = command;
//     }
// }
//
// // Function to select the appropriate LOD based on the distance
// uint SelectLOD(float distance)
// {
//     // Example logic: Assume that there are three LODs and that each LOD has a different distance range
//     if (distance < 100.0f)
//     {
//         return 0;
//     }
//     else if (distance < 200.0f)
//     {
//         return 1;
//     }
//     else
//     {
//         return 2;
//     }
// }
//
// // Function to update the indirect draw command with the new index count and instance count
// void UpdateDrawCommand(inout DrawIndexedInstancedIndirectCommand command, uint LOD)
// {
//     // Example logic: Assume that the index count and instance count are stored in an array based on
//     // Example logic: Assume that the index count and instance count are stored in an array based on the LOD
//     uint indexCounts[3] = { 10, 20, 30 };
//     uint instanceCounts[3] = { 5, 10, 15 };
//
//     command.IndexCount = indexCounts[LOD];
//     command.InstanceCount = instanceCounts[LOD];