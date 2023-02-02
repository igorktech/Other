#pragma once
// MSE

#include <vector>
#include <cmath>
#include <array>
#include <limits>
#include <cstddef>
#include <iostream>

// define a 4x4 matrix
typedef std::array<std::array<double, 4>, 4> Matrix4x4;

// class Vertex
// {
// public:
// 	double x, y, z; // position of vertex
// 	// constructor
// 	Vertex(double x, double y, double z) : x(x), y(y), z(z)
// 	{
// 	}
//
// 	// method to calculate quadric matrix
// 	
// };
struct Vertex
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 color;
	uint16_t vertexId;
};

DirectX::XMFLOAT4X4 quadricMatrix(Vertex v)
{
	DirectX::XMFLOAT4X4 quadric(
		1, v.position.x, v.position.y, v.position.z,
		v.position.x, v.position.x * v.position.x, v.position.x * v.position.y, v.position.x * v.position.z,
		v.position.y, v.position.x * v.position.y, v.position.y * v.position.y, v.position.y * v.position.z,
		v.position.z, v.position.x * v.position.z, v.position.y * v.position.z, v.position.z * v.position.z
	);
	return quadric;
}
class Edge
{
public:
	int v1, v2; // indices of vertices that make up the edge
	// constructor
	Edge(int v1, int v2) : v1(v1), v2(v2)
	{
	}
};
#undef max
double limit = std::numeric_limits<double>::max();
//class QEMSimplification
//{
//public:
//	std::vector<Vertex> vertices;
//	std::vector<Edge> edges;
//	std::vector<Matrix4x4> quadrics;
//
//	// constructor
//	QEMSimplification(std::vector<Vertex> vertices, std::vector<Edge> edges) :
//		vertices(vertices), edges(edges)
//	{
//		// calculate quadric matrix for each vertex
//		for (const Vertex& v : vertices)
//		{
//			quadrics.push_back(quadricMatrix(v));
//		}
//	}
//
//	void simplify(int targetVertices)
//	{
//		while (vertices.size() > targetVertices)
//		{
//			double minError = limit;
//			int minErrorEdge = -1;
//			// find edge with minimum error
//			for (int i = 0; i < edges.size(); i++)
//			{
//				// std::cout << i <<" "<< edges.size()<< std::endl;
//				// std::cout << "vertex size: " << vertices.size() << std::endl;
//
//				int v1 = edges[i].v1;
//				int v2 = edges[i].v2;
//				Matrix4x4 Q = addMatrix(quadrics[v1], quadrics[v2]);
//				double error = calculateError(Q, vertices[v1].position.x, vertices[v1].position.y, vertices[v1].position.z);
//				if (error < minError)
//				{
//					minError = error;
//					minErrorEdge = i;
//				}
//			}
//			// if no edge found, stop the algorithm
//			if (minErrorEdge == -1)
//			{
//				break;
//			}
//			// merge vertices and update edges
//			int v1 = edges[minErrorEdge].v1;
//			int v2 = edges[minErrorEdge].v2;
//			Vertex newVertex = { {float((vertices[v1].position.x + vertices[v2].position.x) / 2.0), float((vertices[v1].position.y + vertices[v2].position.y) / 2.0),
//							 float((vertices[v1].position.z + vertices[v2].position.z) / 2.0)} ,{0,0,0,1} };
//			int newVertexIndex = vertices.size() - 1;
//			vertices.push_back(newVertex);
//			quadrics.push_back(addMatrix(quadrics[v1], quadrics[v2]));
//			// update edges
//			for (int i = 0; i < edges.size(); i++) {
//				if (edges[i].v1 == v1 || edges[i].v2 == v1) {
//					edges[i].v1 = newVertexIndex;
//				}
//				if (edges[i].v1 == v2 || edges[i].v2 == v2) {
//					edges[i].v2 = newVertexIndex;
//				}
//				if (((edges[i].v1 > v2) && (edges[i].v1 < v1)) || ((edges[i].v1 < v2) && (edges[i].v1 > v1))) {
//					edges[i].v1 -= 1;
//				}
//				if (((edges[i].v2 > v2) && (edges[i].v2 < v1)) || ((edges[i].v2 < v2) && (edges[i].v2 > v1))) {
//					edges[i].v2 -= 1;
//				}
//				if ((edges[i].v1 > v2) && (edges[i].v1 > v1))
//				{
//					edges[i].v1 -= 2;
//				}
//				if ((edges[i].v2 > v2) && (edges[i].v2 > v1))
//				{
//					edges[i].v2 -= 2;
//				}
//			}
//			// remove duplicate edges
//			for (int i = 0; i < edges.size(); i++)
//			{
//				for (int j = i + 1; j < edges.size(); j++)
//				{
//					if ((edges[i].v1 == edges[j].v1 && edges[i].v2 == edges[j].v2) || (edges[i].v1 == edges[j].v2 &&
//						edges[i].v2 == edges[j].v1))
//					{
//						// std::cout << "Erased: " <<edges.at(j).v1<<" "<<edges.at(j).v2 << std::endl;
//						edges.erase(edges.begin() + j);
//
//						j--;
//					}
//				}
//			}
//			// remove merged vertex
//			vertices.erase(vertices.begin() + v1);
//			vertices.erase(vertices.begin() + v2 - 1);
//			quadrics.erase(quadrics.begin() + v1);
//			quadrics.erase(quadrics.begin() + v2 - 1);
//		}
//	}
//
//
//	// method to add two matrices
//	Matrix4x4 addMatrix(const Matrix4x4& m1, const Matrix4x4& m2)
//	{
//		Matrix4x4 result;
//		for (int i = 0; i < 4; i++)
//		{
//			for (int j = 0; j < 4; j++)
//			{
//				result[i][j] = m1[i][j] + m2[i][j];
//			}
//		}
//		return result;
//	}
//
//	// method to calculate error
//	double calculateError(const Matrix4x4& Q, double x, double y, double z)
//	{
//		double error = Q[0][0] * x * x + Q[1][1] * y * y + Q[2][2] * z * z + 2 * (Q[0][1] * x * y + Q[0][2] * x * z + Q[
//			1][2] * y * z) + 2 * (Q[0][3] * x + Q[1][3] * y + Q[2][3] * z) + Q[3][3];
//			return error;
//	}
//
//
//};
class Triangle {
public:
	int v1, v2, v3; // indices of vertices that make up the triangle

	// constructor
	Triangle(int v1, int v2, int v3) : v1(v1), v2(v2), v3(v3) {}
};
std::vector<Triangle> constructTriangles(const std::vector<Edge>& edges) {
	std::vector<Triangle> triangles;
	std::vector<std::pair<int, int>> vertexPairs;
	// create vertex pairs from edges
	for (const Edge& e : edges) {
		vertexPairs.push_back(std::make_pair(e.v1, e.v2));
	}
	// loop through vertex pairs and create triangles
	while (!vertexPairs.empty()) {
		int v1 = vertexPairs[0].first;
		int v2 = vertexPairs[0].second;
		int v3 = -1;
		for (int i = 1; i < vertexPairs.size(); i++) {
			if (vertexPairs[i].first == v2) {
				v3 = vertexPairs[i].second;
				break;
			}
			else if (vertexPairs[i].second == v2) {
				v3 = vertexPairs[i].first;
				break;
			}
		}
		if (v3 != -1) {
			triangles.push_back(Triangle(v1, v2, v3));
			vertexPairs.erase(vertexPairs.begin());
			for (int i = 0; i < vertexPairs.size(); i++) {
				if (vertexPairs[i].first == v2 && vertexPairs[i].second == v3) {
					vertexPairs.erase(vertexPairs.begin() + i);
					break;
				}
				else if (vertexPairs[i].first == v3 && vertexPairs[i].second == v2) {
					vertexPairs.erase(vertexPairs.begin() + i);
					break;
				}
			}
		}
		else {
			vertexPairs.erase(vertexPairs.begin());
		}
	}
	return triangles;
}

/*
int main() {
	// create a vector of vertices
	std::vector<Vertex> vertices;
	vertices.push_back(Vertex(1, 1, 1));
	vertices.push_back(Vertex(2, 2, 2));
	vertices.push_back(Vertex(3, 3, 3));
	// create a vector of edges
	std::vector<Edge> edges;
	edges.push_back(Edge(0, 1));
	edges.push_back(Edge(1, 2));
	edges.push_back(Edge(2, 0));
	// create QEMSimplification object
	QEMSimplification simplification(vertices, edges);
	// simplify the model
	simplification.simplify(2);
	return 0;
}
*/

// #include <array>
//
// 		// define a 4x4 matrix
// 		typedef std::array<std::array<double, 4>, 4> Matrix4x4;

Matrix4x4 calculateFaceQuadric(double x1, double y1, double z1, double x2, double y2, double z2, double x3,
	double y3, double z3)
{
	// calculate the normal vector of the face
	double A = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);
	double B = (z2 - z1) * (x3 - x1) - (x2 - x1) * (z3 - z1);
	double C = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
	double D = -(A * x1 + B * y1 + C * z1);

	// calculate the quadric matrix
	Matrix4x4 quadric = {
		{
			{A * A, A * B, A * C, A * D},
			{A * B, B * B, B * C, B * D},
			{A * C, B * C, C * C, C * D},
			{A * D, B * D, C * D, D * D}
		}
	};
	return quadric;
}

// End of MSE

//
//
//cbuffer Constants : register(b0)
//{
//	float4x4 World;
//	float4x4 View;
//	float4x4 Projection;
//};
//
//struct Vertex
//{
//	float3 position;
//	float3 normal;
//	float2 texcoord;
//};
//
//struct Quadric
//{
//	float4x4 matrix;
//};
//
//cbuffer Quadrics : register(b1)
//{
//	Quadric quadrics[100];
//};
//
//cbuffer Edges : register(b2)
//{
//	int edges[200];
//};
//
//struct GS_OUTPUT
//{
//	float4 position : SV_POSITION;
//	float4 color : COLOR;
//};
//
//[maxvertexcount(2)]
//void main(line Vertex input[2], inout LineStream<GS_OUTPUT> output)
//{
//	for (int i = 0; i < 2; i++)
//	{
//		GS_OUTPUT gsOutput;
//		gsOutput.position = mul(mul(mul(float4(input[i].position, 1.0), World), View), Projection);
//
//		// perform QEM algorithm here
//		float minError = 10000.0f;
//		int minErrorEdge = -1;
//		int v1 = edges[input[i].vertexId * 2];
//		int v2 = edges[input[i].vertexId * 2 + 1];
//		float4x4 Q = quadrics[v1] + quadrics[v2];
//		float error = dot(float4(input[i].position, 1), mul(Q, float4(input[i].position, 1)));
//		if (error < minError)
//		{
//			minError = error;
//			minErrorEdge = input[i].vertexId;
//		}
//
//		// assign color based on edge with minimum error
//		gsOutput.color = minErrorEdge == input[i].vertexId ? float4(1, 0, 0, 1) : float4(0, 1, 0, 1);
//
//		output.Append(gsOutput);
//	}
//	output.Restart
