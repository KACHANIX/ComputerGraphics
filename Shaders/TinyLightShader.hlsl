struct ConstantData
{
	float4x4 world;
	float4x4 wvp;
	float4x4 invert_transpose_world;
	float4 viewer_position;
};
struct LightData
{
	float4 direction;
	float4 color;
	float4 ka_spec_pow_ks_x;
};

cbuffer ConstBuf : register(b0)
{
	ConstantData ConstData;
};
cbuffer ConstBuf1 : register(b1)
{
	LightData Lights;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
	float2 tex : TEXCOORD0;
	float4 world_pos : TEXCOORD1;
};

struct Inds
{
	uint pInd;
	uint nInd;
	uint tInd;
};

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

Buffer<float3> Positions : register(t0);
Buffer<float3> Normals : register(t1);
Buffer<float2> TexCoords : register(t2);
StructuredBuffer<Inds> Indexes : register(t3);
 

PS_IN VSMain(uint index : SV_VertexID)
{
	PS_IN output = (PS_IN)0;

	Inds inds = Indexes[index /* + ConstData.indexOffset*/];

	float3 pos = Positions[inds.pInd];
	float3 normal = Normals[inds.nInd];
	float2 tex = TexCoords[inds.tInd];

	output.world_pos = mul(float4(pos, 1.0f), ConstData.world);
	output.pos = mul(float4(pos, 1.0f), ConstData.wvp);
	output.normal = mul(float4(normal, 0.0f), ConstData.world);
	output.tex = tex;

	return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
	float4 diff_val = DiffuseMap.Sample(Sampler, float2(input.tex.x, 1.0f - input.tex.y));
	clip(diff_val.a - 0.01f);

	float3 kd = diff_val.xyz;
	float3 normal = normalize(input.normal.xyz);

	float3 view_dir = normalize(ConstData.viewer_position.xyz - input.world_pos.xyz);
	float3 light_dir = -Lights.direction.xyz;
	float3 ref_vec = normalize(reflect(light_dir,normal));

	float3 diffuse = max(0, dot(light_dir, normal)) * kd;
	float3 ambient = kd * Lights.ka_spec_pow_ks_x.x;
	float3 spec = pow(max(0, dot(-view_dir, ref_vec)), Lights.ka_spec_pow_ks_x.y) * Lights.ka_spec_pow_ks_x.z;
	return float4(Lights.color.xyz * (diffuse + ambient + spec), 1.0f);


	//return color;
}