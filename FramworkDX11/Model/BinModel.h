#pragma once

namespace Fbx { class FbxModel; }

namespace Model
{
	class MeshPart;

	class BinModel
	{
	public:
		friend Fbx::FbxModel;

	private:
		BinModel();
	public:
		BinModel(const BinModel& model);
		~BinModel();

		void Update(void);
		void Render(void);

		bool MousePickked(OUT map<float, MeshPart*>& pickkedParts, D3DXVECTOR3 start, D3DXVECTOR3 direction);

	private:
		void ScanPointMinMax(void);

	public:
		inline string GetFileName(void) { return name; }
		inline void SetFileName(string name) { this->name = name; }

		inline UINT GetMeshPartSize(void) { return meshParts.size(); }
		inline MeshPart* GetMeshPart(UINT index) { assert(index < meshParts.size()); return meshParts[index]; }

		//TRANSFROM
		inline void SetTransform(TRANSFORM tr)				{ transform = tr; }
		inline void SetTransformScale(D3DXVECTOR3 vec)		{ transform.Scale = vec; }
		inline void SetTransformRotateDeg(D3DXVECTOR3 vec)	{ transform.RotationDeg = vec; }
		inline void SetTransformPosition(D3DXVECTOR3 vec)	{ transform.Position = vec; }
		inline void SetTransformTranslate(D3DXVECTOR3 vec)	{ transform.Position += vec; }
		inline TRANSFORM GetTransform(void)					{ return transform; }
		inline D3DXVECTOR3 GetTransformScale(void)			{ return transform.Scale; }
		inline D3DXVECTOR3 GetTransformRotateDeg(void)		{ return transform.RotationDeg; }
		inline D3DXVECTOR3 GetTransformPosition(void)		{ return transform.Position; }
		inline D3DXVECTOR3& GetTransformScaleRef(void)		{ return transform.Scale; }
		inline D3DXVECTOR3& GetTransformRotateDegRef(void)	{ return transform.RotationDeg; }
		inline D3DXVECTOR3& GetTransformPositionRef(void)	{ return transform.Position; }

		inline D3DXMATRIX GetWorld(void) { return world; }

		inline bool GetIsSelected(void)			{ return isSelected; }
		inline void SetisSelected(bool select)	{ isSelected = select; }
		inline bool GetisHovering(void)			{ return isHovering; }
		inline void SetisHovering(bool hover)	{ isHovering = hover;}


	private:
		string name;

		vector<MeshPart*> meshParts;

		D3DXMATRIX world;
		TRANSFORM transform;

		BOX_COLLIDER* boxCollider;

		bool isSelected;
		bool isHovering;
	};
}