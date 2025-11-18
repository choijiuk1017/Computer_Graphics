//==================================================================
//		## ModelLoader ## (assimp 라이브러리를 이용해 모델 정보를 로드한다.)
//==================================================================

#pragma once
#include "include/assimp/Importer.hpp"
#include "include/assimp/scene.h"
#include "include/assimp/postprocess.h"
#include "SkinModel.h"
#include "DeviceManager.h"
#include "MYUTIL.h"
#include "Animation.h"
#include "ModelStructure.h"


#pragma comment(lib, "lib/assimp-vc142-mtd.lib")

class ModelLoader
{
public:
	ModelLoader() {};
	~ModelLoader() {};

	//모델 정보 로드
	SkinModel*	LoadModel(CString path, UINT flag);
	void		LoadAnimation(CString path, SkinModel* model, UINT flag);
protected:
	//노드 정보 생성(본 정보)
	void ProcessNode(aiNode* aiNodeInfo, SkinModel* skModel, NodeInfo* parent = NULL, int depth = 0);
	//메쉬 정보 생성(정점정보, 인덱스 정보, 기본 메쉬 정보)
	void ProcessMesh(aiMesh* mesh, Vertex& vertices, vector<unsigned long>& indices, vector<HierarchyMesh*>& meshList);
	//재질 정보 생성
	void ProcessMaterial(const aiScene* pScene, vector<Material>& matList, CString directoryPath);
	//스키닝 정보 생성(offset_mat, weight)
	void ProcessSkin(aiMesh* aiMesh, HierarchyMesh* mesh, Vertex& vertices, vector<unsigned long>& indices, SkinModel* skModel);
	//애니메이션 정보 생성
	void ProcessAnimation(const aiScene* pScene, SkinModel* skModel);

	//이름을 통해 노드를 찾는다.
	NodeInfo* FindNode(const std::wstring& cleanName, const vector<NodeInfo*>& nodeList);

    void PrintSkeletonTree(const vector<NodeInfo*>& nodeList)
    {
        OutputDebugStringA("==== Skeleton Nodes ====\n");

        for (auto node : nodeList)
        {
            // depth 만큼 들여쓰기
            std::string indent(node->depth * 2, ' ');

            // 유니코드 → ANSI 변환
            std::wstring ws = node->name;
            std::string name(ws.begin(), ws.end());

            char buffer[512];
            sprintf_s(buffer,
                "%s- Node: %s | Depth: %d | localTM[0][0]: %.3f\n",
                indent.c_str(),
                name.c_str(),
                node->depth,
                node->localTM.r[0].m128_f32[0]
            );

            OutputDebugStringA(buffer);
        }
        OutputDebugStringA("========================\n");
    }

    bool IsPreRotationNode(const wstring& name)
    {
        return name.find(L"_$AssimpFbx$_PreRotation") != wstring::npos;
    }

    bool IsTranslationNode(const wstring& name)
    {
        return name.find(L"_$AssimpFbx$_Translation") != wstring::npos;
    }

    std::wstring CleanBoneName(const std::wstring& name);
};
