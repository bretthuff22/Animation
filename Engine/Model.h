#ifndef INCLUDED_MODEL_H
#define INCLUDED_MODEL_H

//=============================================================================
// Forward Declarations
//=============================================================================

struct Bone;
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

	void Render(MeshRenderer& renderer) const;
	
	NONCOPYABLE(Model);

	std::vector<Mesh*> mMeshes;
	
	Bone* mRoot;
	std::vector<Bone*> mBones;
	std::map<std::string, u32> mBoneIndexMap;

	std::vector<MeshBuffer*> mMeshBuffers;
	std::vector<Texture*> mTextures;
};

#endif