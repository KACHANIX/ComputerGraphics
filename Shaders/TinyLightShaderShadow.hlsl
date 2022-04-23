struct ConstantData
{
	float4x4 world;
	float4x4 wvp;
	float4x4 lightvp; //light view projection
	float4 viewer_position; // for specular
};
struct LightData
{
	float4 position; //float4 direction;
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
	float4 light_view_position:TEXCOORD2;
	float3 light_position:TEXCOORD3;
};

struct Inds
{
	uint pInd;
	uint nInd;
	uint tInd;
};

Texture2D DiffuseMap : register(t0);
Texture2D ShadowMap : register(t1);
SamplerState Sampler : register(s0);
SamplerComparisonState Sampler2 : register(s1);

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

	output.pos = mul(float4(pos, 1.0f), ConstData.wvp);
	output.light_view_position = mul(mul(float4(pos,1.0f), ConstData.world),ConstData.lightvp);
	output.normal = mul(float4(normal, 0.0f), ConstData.world);
	output.world_pos = mul(float4(pos, 1.0f), ConstData.world);
	output.light_position = Lights.position.xyz - output.world_pos.xyz; 
	output.tex = tex;

	return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
	float bias = 0.01f;



	float4 diff_val = DiffuseMap.Sample(Sampler, float2(input.tex.x, 1.0f - input.tex.y));
	clip(diff_val.a - bias);
	float3 kd = diff_val.xyz;

	float3 color = kd * Lights.ka_spec_pow_ks_x.x; //default output color is ambient light

	float2 proj_tex_coord;
	proj_tex_coord.x=((input.light_view_position.x/ input.light_view_position.w)+1.0f)/2.0f;
	proj_tex_coord.y=-((input.light_view_position.y/ input.light_view_position.w)+1.0f)/2.0f;
 
 /// wtf 
	float light_depth = input.light_view_position.z / input.light_view_position.w;
	light_depth -= bias;	
	float depth = ShadowMap.SampleCmp(Sampler2, proj_tex_coord.xy, light_depth);

	 
	float3 normal = normalize(input.normal.xyz);
	float3 vec_dir__ = Lights.position.xyz - input.world_pos.xyz;
	float3 light_dir = normalize(vec_dir__);

	float3 diffuse = max(0, dot(light_dir, normal)) * kd * depth; // todo
	float3 view_dir = normalize(ConstData.viewer_position.xyz - input.world_pos.xyz);
	float3 ref_vec = normalize(reflect(light_dir,normal));
	float3 spec = pow(max(0, dot(-view_dir, ref_vec)), Lights.ka_spec_pow_ks_x.y) * Lights.ka_spec_pow_ks_x.z;
	color += diffuse + spec; 


	return float4(Lights.color.xyz * color,1.0f) ;


	// if ((saturate(proj_tex_coord.x) == proj_tex_coord.x) && (saturate(proj_tex_coord.y) == proj_tex_coord.y))
	// {
	// 	float depth = ShadowMap.Sample(Sampler, proj_tex_coord);
	// 	float light_depth = input.light_view_position.z / input.light_view_position.w;
	// 	light_depth -= bias;
	// 	if (light_depth < depth)
	// 	{
	// 		float light_intensity = saturate(dot(input.normal, input.light_position));

	// 		if (light_intensity > 0.0f)
	// 		{ 
	// 			float3 normal = normalize(input.normal.xyz);
	// 			float3 vec_dir__ = Lights.position.xyz - input.world_pos.xyz;
	// 			float3 light_dir = normalize(vec_dir__);
	// 			float3 diffuse = max(0, dot(light_dir, normal)) * kd /** att*/;
				
	// 			float3 view_dir = normalize(ConstData.viewer_position.xyz - input.world_pos.xyz);
	// 			float3 ref_vec = normalize(reflect(light_dir,normal));
	// 			float3 spec = pow(max(0, dot(-view_dir, ref_vec)), Lights.ka_spec_pow_ks_x.y) * Lights.ka_spec_pow_ks_x.z;

	// 			color += diffuse + spec;
	// 		}
	// 	}
	// } 

	// return float4(Lights.color.xyz * color,1.0f) ;
}


PS_IN VSMainLight(uint index : SV_VertexID)
{
	PS_IN output = (PS_IN)0;
	
	Inds inds = Indexes[index /* + ConstData.indexOffset*/];

	float3 pos = Positions[inds.pInd];
	float3 normal = Normals[inds.nInd];
	float2 tex = TexCoords[inds.tInd];

	output.pos = mul(mul(float4(pos, 1.0f), ConstData.world), ConstData.lightvp);
	output.normal = float4(normal, 0.0f);
	output.tex = tex;
	
	return output;
}

float4 PSMainLight( PS_IN input ) : SV_Target
{
	// float4 color = DiffuseMap.Sample(Sampler, float2(input.tex.x, 1.0f - input.tex.y));
	// clip(color.a - 0.01f);
	// return color;
	// return float4(1.0f,1.0f,1.0f,1.0f);
	return float4(1.0f,1.0f,1.0f,0.0f);

}