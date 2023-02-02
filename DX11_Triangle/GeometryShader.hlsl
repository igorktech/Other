#include "def.h"
//struct GSOutput
//{
//	float4 pos : SV_POSITION;
//    float4 color : COLOR;
//};
//struct v2g
//{
//    float4 position : SV_POSITION;
//    float4 color : COLOR;
//};

//struct g2f
//{
//    float4 position : SV_POSITION;
//    float4 color : COLOR;
//    // float2 uv : TEXCOORD1;
//};

////[maxvertexcount(3)]
////void main(
////	triangle v2g input[3] : SV_POSITION,
////	inout TriangleStream< GSOutput > output
////)
////{
////	for (uint i = 0; i < 3; i++)
////	{
////		GSOutput element;
////		element.pos = input[i].position;
////        element.color = input[0].color;
////		output.Append(element);
////	}
////}


//[maxvertexcount(3)]
// void main(triangle v2g input[3], inout TriangleStream<g2f> triStream) //triangle float4 input[3] : SV_POSITION 
//{
//    g2f v;
//    // v.position = input[0].position;
//    // float2 screenPos = FromClipSpace(input[0].position);
//    // float clipPosW = input[0].position.w;

//    for (uint i = 0; i < 3; i++)
//    {
//        g2f element;
//        element.position = input[i].position;
//        element.color = input[0].color;

//        triStream.Append(element);
//    }



//     triStream.RestartStrip();
//}




// cbuffer Constants : register(b0)
// {
//     float4x4 World;
//     float4x4 View;
//     float4x4 Projection;
// };

struct Vertex
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    uint vertexId : TEXCOORD1;
};

struct Quadric
{
    float4x4 mtx;
};

cbuffer Quadrics : register(b0)
{
    float4x4 quadrics[1000];
};

cbuffer Edges : register(b1)
{
    int edges[2000];
};

cbuffer MVPConstantBuffer : register(b2)
{
    float4x4 model;
    float4x4 view;
    float4x4 projection;
};

RWStructuredBuffer<Vertex> position : register(u2);

struct GS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    
};

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
// float4x4 calculateFaceQuadric(double x1, double y1, double z1, double x2, double y2, double z2, double x3,
//                                double y3, double z3)
// {
// 	// calculate the normal vector of the face
//     double A = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);
//     double B = (z2 - z1) * (x3 - x1) - (x2 - x1) * (z3 - z1);
//     double C = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
//     double D = -(A * x1 + B * y1 + C * z1);
//
// 	// calculate the quadric matrix
//     float4x4 quadric =
//     {
//         {
//             { A * A, A * B, A * C, A * D },
//             { A * B, B * B, B * C, B * D },
//             { A * C, B * C, C * C, C * D },
//             { A * D, B * D, C * D, D * D }
//         }
//     };
//     return quadric;
// }

[maxvertexcount(2)]
void main(triangleadj Vertex input[6], inout LineStream<GS_OUTPUT> output)
{
    float3 newPos = (input[0].position + input[1].position) / 2;
    // GS_OUTPUT Output;
    // Output.position = float4(newPos, 1);
    // Output.color = float4(1, 0, 0, 1);
    // output.Append(Output);
    
    bool merge = 0;
    
    for (int i = 0; i < 3
        ; i++)
    {
        GS_OUTPUT gsOutput; //.position = (input[0].position + input[1].position) / 2;
        gsOutput.position = (input[0].position + input[1].position) / 2;
        // if (i != 2 && i != 4)
        {
        //GS_OUTPUT gsOutput;
            
            gsOutput.position = mul(mul(mul(input[i].position, model), view), projection);
    
        // perform QEM algorithm here
            float4 qB = float4(1, 1, 0, 1);
            float minError = 5.00000000f;
            int minErrorEdge = -1;
            int v1 = edges[input[i].vertexId * 2];
            int v2 = edges[input[i].vertexId * 2 + 1];
            float4 newPos = mul(mul(mul(input[i].position, model), view), projection); //() / 2;
            newPos = mul(newPos, model);
            newPos = mul(newPos, view);
            newPos = mul(newPos, projection);

        //float4x4 Q = calculateFaceQuadric();
            float4x4 Q = quadrics[v1] + quadrics[v2];
            float error = dot(input[i].position, mul(Q, input[i].position));

        // gsOutput.position = newPos;
            Q[3][0] = 0;
            Q[3][1] = 0;
            Q[3][2] = 0;
            Q[3][3] = 1;
        // Q = Inv(Q);
        // gsOutput.position =mul(Inv(Q), qB);
        //gsOutput.color = input[i].color;
            if (error < minError)
            {
                minError = error;
                minErrorEdge = input[i].vertexId;

                merge = 0;
                gsOutput.color = float4(1, 0, 0, 1);
            //output.Append(gsOutput);
                gsOutput.position = (input[i].position); // + (input[1].position) / 2);
            //output.Append(gsOutput);
            // gsOutput.position = (input[i].position + input[1].position) / 2;
            
                // output.Append(gsOutput);
            
            }
            else
            {



                const float SINGULAR_THRESHOLD = 1e-11;
  // //           vPos = -mul(SymInv(quadricA), quadricB);
                if (determinant(Q) > SINGULAR_THRESHOLD)
                {
      // gsOutput.position = mul(Inv(Q), qB);
                    gsOutput.position = (
                    input[i + 1].position)









                	;
                    merge = 1;
                    // break;
                }
                // else
                {
                // gsOutput.position = newPos;
                    // gsOutput.position = (input[i].position); // (input[i].position+
                    // input[i+1].position) / 2;

                }
            // gsOutput.position = (input[i + 1].position); /// 2;
        
                gsOutput.color = float4(1, 1, 0, 1);
            //
            //
             // gsOutput.position =input[i].position; //(input[0].position + input[1].position ) / 2;
                // merge = 1;

            //gsOutput.color = float4(1, 1, 0, 1);
            
           // output.Append(gsOutput);
            }
        
        // assign color based on edge with minimum error
        // gsOutput.color = minErrorEdge == input[i].vertexId ? float4(1, 1, 0, 1) : float4(1, 0, 0, 1);
        //minErrorEdge == input[i].vertexId ? float4(1, 1, 0, 1) : float4(1, 0, 0, 1);
        //
        // output.Append(gsOutput);
        }
        output.Append(gsOutput);
    }
    
    // output.RestartStrip();
    
    // for (uint i = 0; i < 2; i++)
    // {
    //     GS_OUTPUT element;
    //     element.position = input[i].position;
    //     element.color = input[0].color;
    //
    //     output.Append(element);
    // }
    //
    //
    //
    // output.RestartStrip();
}


// struct v2g
// {
//     float4 position : SV_POSITION;
//     float4 color : COLOR;
// };
//
// struct g2f
// {
//     float4 position : SV_POSITION;
//     float4 color : COLOR;
//     // float2 uv : TEXCOORD1;
// };
//
// struct FragmentData
// {
//     float4 position : SV_POSITION;
//     float4 quadraticA0 : TEXTCOORD4;
//     float2 quadraticA1 : TEXTCOOTD2;
//     float3 quadraticB : TEXTCOORD3;
//     float quadraticC : TEXTCOORD1;
// };
// float2 FromClipSpace(float4 clipPos)
// {
//     return clipPos.xy / clipPos.w;
// }
//
// float4 ToClipSpace(float2 screenPos, float originalClipPosW)
// {
//     return float4(screenPos.xy * originalClipPosW, 0, originalClipPosW);
// }
//
//
// float3 Expand(float3 vX)
// {
//     return 2 * vX - 1;
// }
// //Map a point to its location in the cluster map array
//
// float2 writeAddr(float3 vPos)
// {
//     // uint iX = clusterId(vPos) / iClusterMapSize.x;
//     // uint iY = clusterId(vPos) % iClusterMapSize.y;
//     // return Expand(float2(iX, iY) / float(iClusterMapSize.x)) + 1.0 / iClusterMapSize.x;
//     return float2(0, 0);
// }
//
// // Computers outer product of arguments
// float3x3 outer(float3 vA, float3 vB)
// {
//     float3x3 mOutput =
//     {
//         vA.x * vB.x, vA.x * vB.y, vA.x * vB.z,
//     	vA.y * vB.x, vA.y * vB.y, vA.y * vB.z,
//     	vA.z * vB.x, vA.z * vB.y, vA.z * vB.z
//     };
//     return mOutput;
// }
//
// // Given a quadratic in (A,b,c) from, packs it into a FragmentData structure
// // suitable for output to the PS for rendering
//
// void packQuadric(float3x3 qA, float3 qB, float qC, inout FragmentData output)
// {
//     output.quadraticA0 = float4(qA[0][0], qA[0][1], qA[0][2], qA[1][1]);
//     output.quadraticA1 = float2(qA[1][2], qA[2][2]);
//     output.quadraticB = qB;
//     output.quadraticC = qC;
// }
//
// [maxvertexcount(3)]
// void main(triangle v2g input[3], inout PointStream<FragmentData> stream)
// {
//    //For the current triangle, compute the area and normal
//     float3 vNormal = (cross(input[1].position - input[0].position, input[2].position - input[0].position));
//     float fArea = length(vNormal) / 6;
//     vNormal = normalize(vNormal);
//
//    //Then compute the distance of plane to the origin along the normal
//     float fDist = -dot(vNormal, input[0].position);
//
//    //Compute the components of the face quadrics using the plane coefficients
//     float3x3 qA = fArea * outer(vNormal, vNormal);
//     float3 qb = fArea * vNormal * fDist;
//     float qc = fArea * fDist * fDist;
//
//     //Loop over each vertex in input triangle primitive
//     for (int i = 0; i < 3; i++)
//     {
//        //Assign the output position in the quadric map
//         FragmentData output;
//         output.position = float4(writeAddr(input[i].position), 0, 1);
//     	// output.color = input[0].color;
//
// 	 //Write the quadric to be accumulated in the quadric map
// 	 // packQuadric(qA, qb, qc, output);
//         output.quadraticA0 = float4(qA[0][0], qA[0][1], qA[0][2], qA[1][1]);
//         output.quadraticA1 = float2(qA[1][2], qA[2][2]);
//         output.quadraticB = qb;
//         output.quadraticC = qc;
//
//         stream.Append(output);
//     }
// }


































//////float3 newPos = (input[0].position + input[1].position) / 2;
//    ////GS_OUTPUT Output;
//    ////Output.position = float4(newPos, 1);
//    ////Output.color = float4(1, 0, 0, 1);
//    ////output.Append(Output);
    
//bool merge = 0;
    
//    for (
//int i = 0;i < 6
//        ; i++)
//    {
//        // if (i != 2 && i != 4)
//        {
//        //GS_OUTPUT gsOutput;
//GS_OUTPUT gsOutput;
//            gsOutput.position = mul(mul(mul(input[i].position, model), view), projection);
    
//        // perform QEM algorithm here
//float4 qB = float4(1, 1, 0, 1);
//float minError = 5.00000000f;
//int minErrorEdge = -1;
//int v1 = edges[input[i].vertexId * 2];
//int v2 = edges[input[i].vertexId * 2 + 1];
//float4 newPos = mul(mul(mul(input[i].position, model), view), projection); //() / 2;
//            newPos = mul(newPos, model);
//            newPos = mul(newPos, view);
//            newPos = mul(newPos, projection);

//        //float4x4 Q = calculateFaceQuadric();
//float4x4 Q = quadrics[v1] + quadrics[v2];
//float error = dot(input[i].position, mul(Q, input[i].position));

//        // gsOutput.position = newPos;
//            Q[3][0] = 0;
//            Q[3][1] = 0;
//            Q[3][2] = 0;
//            Q[3][3] = 1;
//        // Q = Inv(Q);
//        // gsOutput.position =mul(Inv(Q), qB);
//        //gsOutput.color = input[i].color;
//            if (error < minError)
//            {
//                minError =
//error;
//                minErrorEdge = input[i].vertexId;

//                merge = 0;
//                gsOutput.color = float4(1, 0, 0, 1);
//            //output.Append(gsOutput);
//                gsOutput.position = (input[i].position); // + (input[1].position) / 2);
//            //output.Append(gsOutput);
//            // gsOutput.position = (input[i].position + input[1].position) / 2;
            
//                // output.Append(gsOutput);
            
//            }
//            else
//            {



//const float SINGULAR_THRESHOLD = 1e-11;
//  // //           vPos = -mul(SymInv(quadricA), quadricB);
//                if (determinant(Q) > SINGULAR_THRESHOLD)
//                {
//      // gsOutput.position = mul(Inv(Q), qB);
//                    gsOutput.position = (input[i].position + input[i + 1].position + input[i + 2].position) / 3;
//                }
//                else
//                {
//                // gsOutput.position = newPos;
//                    gsOutput.position = (input[i].position); // (input[i].position+
//                    // input[i+1].position) / 2;

//                }
//            // gsOutput.position = (input[i + 1].position); /// 2;
        
//                gsOutput.color = float4(1, 1, 0, 1);
//            //
//            //
//             // gsOutput.position =input[i].position; //(input[0].position + input[1].position ) / 2;
//                merge = 1;

//            //gsOutput.color = float4(1, 1, 0, 1);
            
//           // output.Append(gsOutput);
//            }
        
//        // assign color based on edge with minimum error
//        // gsOutput.color = minErrorEdge == input[i].vertexId ? float4(1, 1, 0, 1) : float4(1, 0, 0, 1);
//        //minErrorEdge == input[i].vertexId ? float4(1, 1, 0, 1) : float4(1, 0, 0, 1);
//        //
//        output.Append(gsOutput);
//        }
//    }
    
//    // output.RestartStrip();


//    // for (uint i = 0; i < 2; i++)
//    // {
//    //     GS_OUTPUT element;
//    //     element.position = input[i].position;
//    //     element.color = input[0].color;
//    //
//    //     output.Append(element);
//    // }
//    //
//    //
//    //
//    // output.RestartStrip();











/*
struct v2g
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

struct g2f
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    // float2 uv : TEXCOORD1;
};

struct FragmentData
{
    float4 position : SV_POSITION;
	float4 quadraticA0 : TEXTCOORD4;
    float2 quadraticA1 : TEXTCOOTD2;
    float3 quadraticB : TEXTCOORD3;
    float quadraticC : TEXTCOORD1;
};
float2 FromClipSpace(float4 clipPos)
{
    return clipPos.xy / clipPos.w;
}

float4 ToClipSpace(float2 screenPos, float originalClipPosW)
{
    return float4(screenPos.xy * originalClipPosW, 0, originalClipPosW);
}

 [maxvertexcount(3)]
// void main(triangle v2g input[3], inout TriangleStream<g2f> triStream) //triangle float4 input[3] : SV_POSITION 
// {
//     g2f v;
//     v.position = input[0].position;
//     float2 screenPos = FromClipSpace(input[0].position);
//     float clipPosW = input[0].position.w;
//
//     for (uint i = 0; i < 3; i++)
// 	{
//         g2f element;
// 		element.position = input[i].position;
//         element.color = input[0].color;
//
//         triStream.Append(element);
//     } 
//
//
//
//     // triStream.RestartStrip();
// }


float3 Expand( float3 vX)
{
    return 2 * vX - 1;
}
//Map a point to its location in the cluster map array

float2 writeAddr(float3 vPos)
{
    // uint iX = clusterId(vPos) / iClusterMapSize.x;
    // uint iY = clusterId(vPos) % iClusterMapSize.y;
    // return Expand(float2(iX, iY) / float(iClusterMapSize.x)) + 1.0 / iClusterMapSize.x;
    return float2(0, 0);
}

// Computers outer product of arguments
float3x3 outer( float3 vA, float3 vB)
{
    float3x3 mOutput =
    {
        vA.x * vB.x, vA.x * vB.y, vA.x * vB.z,
    	vA.y * vB.x, vA.y * vB.y, vA.y * vB.z,
    	vA.z * vB.x, vA.z * vB.y, vA.z * vB.z
    };
    return mOutput;
}

// Given a quadratic in (A,b,c) from, packs it into a FragmentData structure
// suitable for output to the PS for rendering

void packQuadric(float3x3 qA, float3 qB, float qC, inout FragmentData output)
{
    output.quadraticA0 = float4(qA[0][0], qA[0][1], qA[0][2], qA[1][1]);
    output.quadraticA1 = float2(qA[1][2], qA[2][2]);
    output.quadraticB = qB;
    output.quadraticC = qC;
}

[maxvertexcount(3)]
void main(triangle v2g input[3], inout PointStream<FragmentData> stream)
{
   //For the current triangle, compute the area and normal
    float3 vNormal = (cross(input[1].position - input[0].position, input[2].position - input[0].position));
    float fArea = length(vNormal) / 6;
    vNormal = normalize(vNormal);

   //Then compute the distance of plane to the origin along the normal
    float fDist = -dot(vNormal, input[0].position);

   //Compute the components of the face quadrics using the plane coefficients
    float3x3 qA = fArea * outer(vNormal, vNormal);
    float3 qb = fArea * vNormal * fDist;
    float qc = fArea * fDist * fDist;

    //Loop over each vertex in input triangle primitive
    for (int i = 0; i < 3; i++)
    {
       //Assign the output position in the quadric map
        FragmentData output;
        output.position = float4(writeAddr(input[i].position), 0, 1);
    	// output.color = input[0].color;

	 //Write the quadric to be accumulated in the quadric map
	 // packQuadric(qA, qb, qc, output);
        output.quadraticA0 = float4(qA[0][0], qA[0][1], qA[0][2], qA[1][1]);
        output.quadraticA1 = float2(qA[1][2], qA[2][2]);
        output.quadraticB = qb;
        output.quadraticC = qc;

        stream.Append(output);
    }
}
*/