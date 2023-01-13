// struct GSOutput
// {
// 	float4 pos : SV_POSITION;
// };
//
// [maxvertexcount(3)]
// void main(
// 	triangle float4 input[3] : SV_POSITION, 
// 	inout TriangleStream< GSOutput > output
// )
// {
// 	for (uint i = 0; i < 3; i++)
// 	{
// 		GSOutput element;
// 		element.pos = input[i];
// 		output.Append(element);
// 	}
// }

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

// [maxvertexcount(3)]
// void main(/*triangle float4 input[3] : SV_POSITION */triangle v2g input[3], inout TriangleStream<g2f> triStream)
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
