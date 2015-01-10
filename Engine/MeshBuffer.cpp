//====================================================================================================
// Filename:	MeshBuffer.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "MeshBuffer.h"

#include "GraphicsSystem.h"
#include "Mesh.h"
#include "MeshUtil.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

MeshBuffer::MeshBuffer()
	: mpVertexBuffer(nullptr)
	, mpIndexBuffer(nullptr)
	, mVertexFormat(0)
	, mVertexSize(0)
	, mVertexCount(0)
	, mIndexCount(0)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------
MeshBuffer::~MeshBuffer()
{
	ASSERT(mpVertexBuffer == nullptr, "[MeshBuffer] Vertex buffer not released!");
	ASSERT(mpIndexBuffer == nullptr, "[MeshBuffer] Index buffer not released!");
}

//----------------------------------------------------------------------------------------------------

void MeshBuffer::Initialize(GraphicsSystem& gs, u32 vertexFormat, const void* vertexData, u32 numVertices)
{
	mVertexFormat = vertexFormat;
	mVertexCount = numVertices;
	
	mVertexSize = MeshUtil::ComputeVertexSize(vertexFormat);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = mVertexSize * mVertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertexData;

	gs.GetDevice()->CreateBuffer(&bd, &initData, &mpVertexBuffer);
}

//----------------------------------------------------------------------------------------------------

void MeshBuffer::Initialize(GraphicsSystem& gs, u32 vertexFormat, const void* vertexData, u32 numVertices, const u16* indexData, u32 numIndices)
{
	Initialize(gs, vertexFormat, vertexData, numVertices);
	
	mIndexCount = numIndices;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = 2 * mIndexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = indexData;

	gs.GetDevice()->CreateBuffer(&bd, &initData, &mpIndexBuffer);
}

//----------------------------------------------------------------------------------------------------

void MeshBuffer::Initialize(GraphicsSystem& gs,u32 vertexFormat, const Mesh& mesh)
{
	Initialize(gs, vertexFormat, mesh.GetVertices(), mesh.GetVertexCount(), mesh.GetIndices(), mesh.GetIndexCount());
}

//----------------------------------------------------------------------------------------------------

void MeshBuffer::Terminate()
{
	SafeRelease(mpVertexBuffer);
	SafeRelease(mpIndexBuffer);
}

//----------------------------------------------------------------------------------------------------

void MeshBuffer::Render(GraphicsSystem& gs)
{
	// Set vertex buffer
	UINT stride = mVertexSize;
	UINT offset = 0;
	gs.GetContext()->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);

	// Set primitive topology
	gs.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Check if we are using index buffer
	if (mpIndexBuffer != nullptr)
	{
		// Set index buffer
		gs.GetContext()->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

		// Draw indexed mesh
		gs.GetContext()->DrawIndexed(mIndexCount, 0, 0 );
	}
	else
	{
		// Draw mesh
		gs.GetContext()->Draw(mVertexCount, 0);
	}
}