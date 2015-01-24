#ifndef INCLUDED_MODEL_H
#define INCLUDED_MODEL_H

//=============================================================================
// Forward Declarations
//=============================================================================

class GraphicsSystem;
class Mesh; 
class MeshBuffer;
class MeshRenderer;
class Texture;

//=============================================================================
// Class Declarations
//=============================================================================

class Model 
{
public:
	Model();
	~Model();

	void Load(GraphicsSystem& gs, const char* pFileName);
	void Unload();

	void Render(MeshRenderer& renderer);
	
	NONCOPYABLE(Model);

	std::vector<Mesh*> mMeshes;
	std::vector<MeshBuffer*> mMeshBuffers;
	std::vector<Texture*> mTextures;
};

#endif