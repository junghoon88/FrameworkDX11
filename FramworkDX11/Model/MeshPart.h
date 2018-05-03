#pragma once

namespace Fbx { struct MeshData; class FbxModel; }

namespace Model
{
	class BinModel;

	class MeshPart
	{
		class MeshBuffer : public ShaderBuffer
		{
		public:
			MeshBuffer() : ShaderBuffer(&Data, sizeof(Struct))
			{
				Data.color = D3DXCOLOR(1, 1, 1, 1);
				Data.viewMode = 0;
			}

			MeshBuffer(const MeshBuffer& buffer) : ShaderBuffer(&Data, sizeof(Struct))
			{
				Data.color = buffer.Data.color;
				Data.viewMode = buffer.Data.viewMode;
			}

			struct Struct
			{
				D3DXCOLOR color;
				
				UINT viewMode;
				float padding[3];
			} Data;
		};

	public:
		friend Fbx::MeshData;
		friend Fbx::FbxModel;

	public:
		MeshPart(Fbx::MeshData& data);
		MeshPart(const MeshPart& part);
		~MeshPart();

		void Update(void);
		void Render(void);
		void Render(bool bSelected, bool bHovered);


	private:
		void CreateBuffer(void);
		void CreateRasterizer(void);
		void ScanPointMinMax(void);

	public:
		bool MousePickked(D3DXVECTOR3 start, D3DXVECTOR3 direction, OUT float& dist);

	public:
		inline void SetParentModel(BinModel* model) { parentModel = model; }
		inline BinModel* GetParentModel(void)	    { return parentModel; }


		inline D3DXCOLOR* GetColor(void)			{ return &meshBuffer->Data.color; }
		inline void SetColor(D3DXCOLOR color)	{ meshBuffer->Data.color = color; }

		inline string GetName(void) { return name; }

		inline D3DXMATRIX GetWorld(void) { return world; }
		inline void SetWorld(D3DXMATRIX& world) { this->world = world; }

		inline Material* GetMaterial(void) { return material; }


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

		inline bool GetIsSelected(void) { return isSelected; }
		inline void SetisSelected(bool select) { isSelected = select; }
		inline bool GetisHovering(void) { return isHovering; }
		inline void SetisHovering(bool hover) { isHovering = hover; }

		inline BOX_COLLIDER* GetBoxCollider(void) { return boxCollider; }

	private:
		//외부에서 받아옴
		BinModel* parentModel;

		//
		string name;
		Material* material;
		D3DXMATRIX world;
		D3DXMATRIX matLocal;

		WorldBuffer* worldBuffer;
		MeshBuffer* meshBuffer;

		vector<VertexTextureNormal> vertexData;
		vector<UINT>				indexData;

		ID3D11Buffer*				vertexBuffer;
		ID3D11Buffer*				indexBuffer;

		ID3D11RasterizerState*		rasterizerDefault;
		ID3D11RasterizerState*		rasterizerWireframe;


		TRANSFORM transform;

		BOX_COLLIDER* boxCollider;

		bool isSelected;
		bool isHovering;

	};
}