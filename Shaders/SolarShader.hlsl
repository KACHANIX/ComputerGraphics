struct ConstantData
{
	float4x4 WorldViewProj;
};
 

cbuffer ConstBuf : register(b0)
{
	ConstantData ConstData;
};
 

struct VS_IN
{
	float4 pos : POSITION;
	float4 col : COLOR;
};


struct PS_IN
{
	float4 pos : SV_POSITION;
 	float4 col : COLOR;
};
 
PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
	
	output.pos = mul(float4(input.pos.xyz, 1.0f), ConstData.WorldViewProj);
	output.col = input.col;
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
	float4 col = input.col;
#ifdef TEST
	if (input.pos.x > 400) col = TCOLOR;
#endif
	return col;
}