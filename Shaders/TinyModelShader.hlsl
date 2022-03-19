struct ConstantData
{
	float4x4 WorldViewProj;
};
 

cbuffer ConstBuf : register(b0)
{
	ConstantData ConstData;
};
 

struct PS_IN
{
	float4 pos : SV_POSITION;
 	float4 col : COLOR;
 	float2 tex : TEXCOORD;
};

struct Inds
{
	uint pInd;
	uint nInd;
	uint tInd;
}

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

Buffer<float3> Positions : register(t0);
Buffer<float3> Normals : register(t1);
Buffer<float2> TexCoords : register(t2);
StructuredBuffer<Inds> Indexes : register(t3); 


struct VS_IN
{
	float4 pos : POSITION;
	float4 col : COLOR;
};


PS_IN VSMain(unit index : SV_VertexID)
{
	PS_IN output = (PS_IN)0;
	
	Inds inds = Indexes[index /* + ConstData.indexOffset*/];

	float3 pos = Positions[inds.pInd];
	float3 normal = Positions[inds.nInd];
	float2 tex = Positions[inds.tInd];

	output.pos = mul(float4(pos, 1.0f), ConstData.WorldViewProj);
	output.normal = float4(normal, 0.0f);
	output.col = tex;
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
	float4 color = DiffuseMap.Sample(Sampler, float2(input.tex.x, 1.0f - input.tex.y));
	clip(color.a - 0.01f);
	return color;
}